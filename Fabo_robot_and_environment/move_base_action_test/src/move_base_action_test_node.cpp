#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>* ac;
int main(int argc, char** argv)
{
  ros::init(argc, argv, "movebase_client");

  // 创建一个SimpleActionClient，它可以向move_base发送目标点
  // actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);
  ac = new actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>("move_base", true);

  // 等待连接到move_base的服务器
  ROS_INFO("Waiting for the move_base action server to come up ...");
  ac->waitForServer();

  // 创建一个move_base_msgs::MoveBaseGoal，设置要发送的目标点
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = -4.0;
  goal.target_pose.pose.position.y = 0.0;
  goal.target_pose.pose.orientation.w = 1.0;
  goal.target_pose.pose.orientation.x = 0.0;
  goal.target_pose.pose.orientation.y = 0.0;
  goal.target_pose.pose.orientation.z = 0.0;

  // 发送目标点
  ROS_INFO("publish action goal for move_base...");
  ac->sendGoal(goal);

  // 等待move_base执行任务
  ac->waitForResult();

  // 输出任务执行结果
  if(ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("The robot reached the goal!");
  else
    ROS_INFO("The base failed to reach the goal!...");

  return 0;
}
