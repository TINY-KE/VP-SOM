/*
 * @Descripttion: 
 * @version: 
 * @Author: Zhang Jiadong
 * @Date: 2022-08-24 20:58:00
 * @LastEditors: GetOverMassif 164567487@qq.com
 * @LastEditTime: 2022-09-20 21:47:08
 */

//ros头文件
#include <ros/ros.h>
#include "std_msgs/String.h"
#include <tf/transform_broadcaster.h>

#include "BtManager.h"
// #include "BtRecall.h"
// 蓝牙通信  TODO: 无线通讯通道
#include "serial.h"

using namespace std;
using namespace FABO_ROBOT;

// 蓝牙
#define PORT "/dev/rfcomm0"
// #define PORT "/dev/ttyUSB0"
#define BAUDRATE 115200
int ret;
pthread_t th;

//创建接收线程，用于读取串口数据
char port[] = {PORT};
int bluetooth = open_serial(port, BAUDRATE, 8, 'N', 1);

/******************************************************************
 * 一、BtRecall函数：  btRecall会不断的接收机器人发送的信息，并存储在btRecall中的成员变量中。
*******************************************************************/

int main(int argc, char** argv){
   
    // ROS node
    ros::init(argc, argv, "cmdvel_to_blueteeth");
    ros::NodeHandle n;
    printInColor("启动cmd_vel转译\n\n", GREEN);

    
/* 将cmd_vel, 转换为左右轮的速度, 并用蓝牙发送出去. */
    FABO_ROBOT::BtManager btmanager(bluetooth);
    btmanager.set_ros_node(n);

    

// /* 发布 odom 和 map, 之间的虚假静态tf */
//     ros::Rate r(10);
//     tf::TransformBroadcaster broadcaster;
//     while(n.ok()){
//         broadcaster.sendTransform(
//         tf::StampedTransform(
//             tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0, 0.0, 0)),
//             ros::Time::now(),"map", "odom"));
//         r.sleep();
//     }
    ros::spin();

    return 0;
}
