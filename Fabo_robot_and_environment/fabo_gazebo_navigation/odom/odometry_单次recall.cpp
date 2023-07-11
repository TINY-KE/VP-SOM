#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

// ros
ros::Publisher odom_pub;
geometry_msgs::TransformStamped odom_trans;
ros::Time current_time;
ros::Time last_time;
geometry_msgs::Twist cmd_vel;
// tf::TransformBroadcaster broadcaster;
tf::TransformBroadcaster * broadcaster = NULL;

// initial position
double x = 0.0; 
double y = 0.0;
double th = 0;

// velocity
double vx = 0.4;
double vy = 0.0;
double vth = 0.4;

const double degree = M_PI/180;

void cmdvelCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
  	ROS_INFO("I heard: cmd vel %f %f", msg->linear.x, msg->angular.z);
	//   cmd_vel = *msg;

	current_time = ros::Time::now(); 
	vx = msg->linear.x;
	vy = msg->angular.z;
	double dt = (current_time - last_time).toSec();
	double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
	double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
	double delta_th = vth * dt;

	x += delta_x;
	y += delta_y;
	th += delta_th;

	geometry_msgs::Quaternion odom_quat;	
	odom_quat = tf::createQuaternionMsgFromRollPitchYaw(0,0,th);

	// update transform
	odom_trans.header.stamp = current_time; 
	odom_trans.transform.translation.x = x; 
	odom_trans.transform.translation.y = y; 
	odom_trans.transform.translation.z = 0.0;
	odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(th);

	//filling the odometry
	nav_msgs::Odometry odom;
	odom.header.stamp = current_time;
	odom.header.frame_id = "odom";
	odom.child_frame_id = "base_footprint";

	// position
	odom.pose.pose.position.x = x;
	odom.pose.pose.position.y = y;
	odom.pose.pose.position.z = 0.0;
	odom.pose.pose.orientation = odom_quat;

	//velocity
	odom.twist.twist.linear.x = vx;
	odom.twist.twist.linear.y = vy;
	odom.twist.twist.linear.z = 0.0;
	odom.twist.twist.angular.x = 0.0;
	odom.twist.twist.angular.y = 0.0;
	odom.twist.twist.angular.z = vth;

	last_time = current_time;

	// publishing the odometry and the new tf
	broadcaster->sendTransform(odom_trans);
	odom_pub.publish(odom);

}


int main(int argc, char** argv) {
	
	ros::init(argc, argv, "state_publisher");
	ros::NodeHandle n;
	broadcaster = new(tf::TransformBroadcaster);
	odom_pub = n.advertise<nav_msgs::Odometry>("odom", 10);
	current_time = ros::Time::now();
	last_time = ros::Time::now();

	ros::Subscriber sub_cmd_vel   = n.subscribe("/cmd_vel", 1000, cmdvelCallback);

	ros::spin();
	return 0;
}
