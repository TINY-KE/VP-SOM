/**
 * 该例程监听tf数据，并计算、发布turtle2的速度指令
 */
 
#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include<chrono>
#include <eigen3/Eigen/Dense>
// #include<Eigen/Dense>
//eigen cv的convert
#include<opencv2/core/core.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/core/eigen.hpp>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

Eigen::Matrix4d Quation2Eigen(const double qx, const double qy, const double qz, const double qw, const double tx,
                                 const double ty, const double tz) {

    Eigen::Quaterniond quaternion(Eigen::Vector4d(qx, qy, qz, qw));
    Eigen::AngleAxisd rotation_vector(quaternion);
    Eigen::Isometry3d T = Eigen::Isometry3d::Identity();
    T.rotate(rotation_vector);
    T.pretranslate(Eigen::Vector3d(tx, ty, tz));
    Eigen::Matrix4d Pose_eigen = T.matrix();
    return Pose_eigen;
}

cv::Mat toCvMat(const Eigen::Matrix<double,4,4> &m)
{
    cv::Mat cvMat(4,4,CV_32F);
    for(int i=0;i<4;i++)
        for(int j=0; j<4; j++)
            cvMat.at<float>(i,j)=m(i,j);

    return cvMat.clone();
    //cv::Mat cv_mat_32f;
    //cv::eigen2cv(GroundtruthPose_eigen, cv_mat_32f);
    //cv_mat_32f.convertTo(mT_body_cam, CV_32F);
}

cv::Mat Quation2CvMat(const double qx, const double qy, const double qz, const double qw, const double tx, const double ty, const double tz  ) {
    return toCvMat(
            Quation2Eigen(qx, qy, qz, qw, tx, ty, tz )
    );
}

actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>* ac;

int main(int argc, char** argv)
{
	// 初始化ROS节点
	ros::init(argc, argv, "my_tf_listener");
 
    // 创建节点句柄
	ros::NodeHandle node;
    
    ac= new actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>("move_base", true);
	// // 请求产生turtle2
	// ros::service::waitForService("/spawn");
	// ros::ServiceClient add_turtle = node.serviceClient<turtlesim::Spawn>("/spawn");
	// turtlesim::Spawn srv;
	// add_turtle.call(srv);
 
	// 创建发布turtle2速度控制指令的发布者
	// ros::Publisher turtle_vel = node.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 10);
 
	// 创建tf的监听器
	tf::TransformListener listener;
    ac = new actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>("move_base", true);

	ros::Rate rate(10.0);
    
    cv::Mat T_w_basefootprint;

	while (node.ok())
	{
		// 获取turtle1与turtle2坐标系之间的tf数据
		tf::StampedTransform transform;
		try
		{
			std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
            listener.waitForTransform("/map", "/base_footprint", ros::Time(0), ros::Duration(1.0));
            listener.lookupTransform("/map", "/base_footprint", ros::Time(0), transform);
            std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
            double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >((t2 - t1)*1000).count();
            std::cout<<"处理一次的时间: "<<ttrack<<"ms"<<std::endl;

            T_w_basefootprint = Quation2CvMat(
                        transform.getRotation().x(),
                        transform.getRotation().y(),
                        transform.getRotation().z(),
                        transform.getRotation().w(),
                        transform.getOrigin().x(),
                        transform.getOrigin().y(),
                        0.0  //transform.getOrigin().x(),
                );

            while(!ac->waitForServer(ros::Duration(5.0))){
                    ROS_INFO("Waiting for the move_base action server to come up");
                }

            // 创建一个move_base_msgs::MoveBaseGoal，设置要发送的目标点
            move_base_msgs::MoveBaseGoal goal;
            goal.target_pose.header.frame_id = "map";
            goal.target_pose.header.stamp = ros::Time::now();
            goal.target_pose.pose.position.x = transform.getOrigin().x() - 1.0;
            goal.target_pose.pose.position.y = transform.getOrigin().y() ;
            goal.target_pose.pose.position.z = 0;
            goal.target_pose.pose.orientation.w = transform.getRotation().w();
            goal.target_pose.pose.orientation.x = transform.getRotation().x();
            goal.target_pose.pose.orientation.y = transform.getRotation().y();
            goal.target_pose.pose.orientation.z = transform.getRotation().z();

            // 发送目标点
            ROS_INFO("publish action goal for move_base...");
            ac->sendGoal(goal);

            // 等待move_base执行任务
            ac->waitForResult();
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
			continue;
		}
 
        

		// rate.sleep();
	}
	return 0;
};