#include "BtManager.h"

namespace FABO_ROBOT
{

BtManager::BtManager(int &blueteeth_): blueteeth(blueteeth_)
{
}

void BtManager::set_ros_node(ros::NodeHandle& n){
    // on cmd_vel
    sub_cmdvel = n.subscribe("/cmd_vel", 1000, &BtManager::callback, this);
    sub_cmdVel_keybord = n.subscribe("/turtle1/cmd_vel", 1, &BtManager::cmdVel_callback, this);
    sub_head = n.subscribe("/mam_angle", 1, &BtManager::head_callback, this);
}

void BtManager::cmdVel_callback(const geometry_msgs::Twist& msg) {
    // normal : vl = 2/-2, z = 2/-2, D = 500, kl = 0.01, ka = 1.5
    // vl = kl * (v_left + v_right) / 2, va = ka * (v_right - v_left) / D
    float D = 500.0, kl = 0.01, ka = 2.0;
    float vl = msg.linear.x, va = msg.angular.z /5;
    float v_sum = (vl * 2.0 / kl), v_dif = va * D / ka;
    int v_left = (int)(v_sum - v_dif) / 2;
    int v_right = (int)(v_sum + v_dif) / 2;
    // wheelMotion(v_left, v_right, 600);
    double  timeStamp = ros::Time::now().toSec();

    string order = "Wheel," + std::to_string(v_left) +
                        "," + std::to_string(v_right) +
                        "," + "100"+
                        "," + std::to_string(timeStamp);
    cout << "order = " << order << endl;
    sendBtData(order);
}

void BtManager::head_callback(const std_msgs::Float64& msg) {
    int angle;
    angle =  msg.data/*逆时针-90~90*/  *(-1) +90/*顺时针0~180*/ + 30;
    
    int vel = 50;
    
    string order = "Head," + std::to_string(angle) +
                        "," + std::to_string(vel);
    cout << "order = " << order << endl;
    sendBtData(order);
}

void BtManager::sendBtData(string str){
    int length = str.size();
    char *temp = (char*)str.c_str();
    char str_char[length + 2];
    strcpy(str_char, temp);

    // print out the data
    printInColor("正在发送蓝牙数据 : ", BLUE);
    printInColor(str_char, BLUE);
    printf("\n");

    write(blueteeth, str_char, strlen(str_char));

    printInColor("发送完成\n\n", BLUE);
}

// private
void BtManager::compute(const double linear_x, const  double angular_z ,string& wheel_speed_r, string& wheel_speed_l ){

    double width_robot = 0.447 ; //447mm  TODO:  速度的单位是m/s ???

    //copy from https://blog.csdn.net/qq_34935373/article/details/107605615
    // 订阅cmd_val下的geometry_msgs::Twist消息 ，并且实际转化为左右轮的速度，以下是转换的源码。
    double vel_x = linear_x;
    double vel_th = angular_z;
    double right_vel = 0.0;
    double left_vel = 0.0;
    left_vel = vel_x - vel_th * width_robot / 2.0;  left_vel = left_vel *1000.0 *1.0585;  /* TODO: 其实标定出的结果是1.0385 */
    right_vel = vel_x + vel_th * width_robot / 2.0; right_vel = right_vel*1000.0 *1.0759;
 
    
    wheel_speed_l = to_string(left_vel);
    wheel_speed_r = to_string(right_vel);
}

void BtManager::callback(const geometry_msgs::Twist &msg){
    std::string wheel_speed_r = "";
    std::string wheel_speed_l = "";
    // bluetooth_message = bluetooth_message                     + msg.name + ","                 + to_string(msg.current_phase) + "/" + to_string(msg.total_phase);
    compute( msg.linear.x,  msg.angular.z  ,  wheel_speed_r,  wheel_speed_l);
    int time = 200; //cmd_vel的持续时长.单位ms
    double  timeStamp = ros::Time::now().toSec();
    string move_order = "Wheel," + wheel_speed_l +   "," + wheel_speed_r + "," + std::to_string(time)+
                                "," + std::to_string(timeStamp);
    cout << "cmdvel.x=" << msg.linear.x << " cmdvel.ywa=" << msg.angular.z <<" ,order = " << move_order << endl;
    // printInColor("蓝牙发送:", RED, wheel_speed_l , wheel_speed_r);
    sendBtData(move_order);
    return;
}

};
