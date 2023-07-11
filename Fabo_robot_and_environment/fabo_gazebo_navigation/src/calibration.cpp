// 


#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <tf2/utils.h>
#include <tf/transform_datatypes.h>
int main(int argc, char** argv){
    ros::init(argc, argv, "calibration_tf_listener");
    ros::NodeHandle node;
    
    ros::service::waitForService("spawn");
    ros::ServiceClient add_turtle =
    node.serviceClient<turtlesim::Spawn>("spawn");
    turtlesim::Spawn srv;
    add_turtle.call(srv);


    //创建TransformListener类监听对象
    tf::TransformListener listener;  
    int i = 0;
    ros::Rate rate(10.0);
    int op;
    ROS_INFO("Input any key, to get TF");
    
    while (node.ok() ){
        std::cin >> op;
        if(op != NULL){
            ROS_INFO("Compute TF");
            //创建一个StampedTransform对象存储变换结果数据
            tf::StampedTransform transform;
            tf::StampedTransform transform_old;
            
            //监听包装在一个try-catch块中以捕获可能的异常
            try{
            //向侦听器查询特定的转换，想要转换的时间ros::Time(0)提供了最新的可用转换。
                listener.lookupTransform("/map", "/base_footprint",     ros::Time(0), transform);
                if(i == 0) transform_old = transform;
                i++;
                double angle = (tf::getYaw(transform_old.getRotation()) - tf::getYaw(transform.getRotation())) * 180.0 / 3.1415926;
                double x_diff = transform_old.getOrigin().x() - transform.getOrigin().x();
                double y_diff = transform_old.getOrigin().y() - transform.getOrigin().y();
                ROS_INFO("x:%f,  y:%f,  angle:%f", x_diff , y_diff , angle);
            }
            catch (tf::TransformException &ex) {
                ROS_ERROR("%s",ex.what());
                ros::Duration(1.0).sleep();
                continue;
            }
            
            transform_old = transform;
            op = NULL;
            rate.sleep();
            ROS_INFO("Input any key, to get TF");
        }
    }
    return 0;
};
