#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>


geometry_msgs::PoseWithCovariance pose_with_covariance;
geometry_msgs::Twist cmd_vel;


void initPoseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
{
  ROS_INFO("I heard: pose with stamp %i", msg->header.stamp);
  pose_with_covariance = msg->pose;
}

void cmdvelCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
  ROS_INFO("I heard: cmd vel %f %f", msg->linear.x, msg->angular.z);
  cmd_vel = *msg;
}

int main(int argc, char** argv){
  ros::init(argc, argv, "odometry_publisher");

  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);

  ros::Subscriber sub_init_pose = n.subscribe("/initialpose", 1000, initPoseCallback);
  ros::Subscriber sub_cmd_vel   = n.subscribe("/cmd_vel", 1000, cmdvelCallback);

  tf::TransformBroadcaster odom_broadcaster;


  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  pose_with_covariance.pose.position = geometry_msgs::Point();
  pose_with_covariance.pose.orientation = tf::createQuaternionMsgFromYaw(0);

  ros::Rate r(5);
  double control_frequency = 0.2;


  while(n.ok()){

    auto current_pose = pose_with_covariance.pose;

    double x = current_pose.position.x + control_frequency*cmd_vel.linear.x * cos(tf::getYaw(current_pose.orientation));
    double y = current_pose.position.y + control_frequency*cmd_vel.linear.x * sin(tf::getYaw(current_pose.orientation));
    double yaw = tf::getYaw(current_pose.orientation) + control_frequency*cmd_vel.angular.z;

    double vx = cmd_vel.linear.x;
    double vy = 0.0;
    double vth = cmd_vel.angular.z;

    current_time = ros::Time::now();

    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(yaw);

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_footprint";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;//odom_quat;

    //set the velocity
    odom.child_frame_id = "base_footprint";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //publish the message
    odom_pub.publish(odom);
    ROS_INFO("robot location to odom: %f %f %f", x ,y, yaw);
    last_time = current_time;
    pose_with_covariance.pose = odom.pose.pose;

    ros::spinOnce();               // check for incoming messages
    r.sleep();
  }
}