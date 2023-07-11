/*
 * @Descripttion: 
 * @version: 
 * @Author: Zhang Jiadong
 * @Date: 2021-12-29 11:40:12
 * @LastEditors: GetOverMassif 164567487@qq.com
 * @LastEditTime: 2022-08-28 16:49:10
 */

#ifndef BTMANAGER_H
#define BTMANAGER_H

#include "common_include.h"
#include <string>
// #include "BtRecall.h"
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64.h>
// #include <thread>
#include <mutex>

using namespace std;

class BtRecall;

namespace FABO_ROBOT
{

class BtManager{
public:
    BtManager(int &blueteeth_);

    ~BtManager(){}

    void set_ros_node(ros::NodeHandle& n);
    
    void sendBtData(string str);

    void processBtData(string btData_str);

    /**
     * @brief 通过蓝牙发送指令，并且发布对机械臂的控制
     * 
     * @param parameter_blueteeth 需要通过蓝牙发送的字符串（string）
     * @param arm_action 调用机械臂的动作（string）
     * @param arm_rate 
     */
    void sendBtMsg(double r_v, double l_v, double time);

private:
    int blueteeth;
    std::mutex mutexPub;
    BtRecall *mBtRecall;

    //待定
    ros::Publisher pub_;
    // 接受cmd_vel
    ros::Subscriber sub_cmdvel;
    ros::Subscriber  sub_cmdVel_keybord ;
    ros::Subscriber  sub_head ;
    void callback(const geometry_msgs::Twist &msg);
    void cmdVel_callback(const geometry_msgs::Twist& msg) ;
    void head_callback(const std_msgs::Float64& msg) ;
    void compute(const double linear_x,const  double angular_z ,string& r_v, string& l_v );

};

}

#endif
