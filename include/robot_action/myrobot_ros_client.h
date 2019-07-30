/*
 * myrobot_ros_client.h
 *
 *  Created on: Jul 30, 2019
 *      Author: kukauser
 */

#ifndef ROBOT_ACTION_INCLUDE_ROBOT_ACTION_MYROBOT_ROS_CLIENT_H_
#define ROBOT_ACTION_INCLUDE_ROBOT_ACTION_MYROBOT_ROS_CLIENT_H_

#include "robot_action/myrobot.h"
#include <ros/ros.h>
#include <robot_action/MoveRobotAction.h>
#include <actionlib/server/simple_action_server.h>

namespace robot_action{

class MyRobotROSClient
{
public:
  MyRobotROSClient(ros::NodeHandle nh, MyRobot& myRobot);


private:
  MyRobot& myRobot_;
  actionlib::SimpleActionServer<robot_action::MoveRobotAction> action_server_;
  robot_action::MoveRobotFeedback feedback_;
  robot_action::MoveRobotResult result_;


  void NewGoalCallback();
  void FeedbackCallback(double current_position);
  void ResultCallback(double end_position);
};









}



#endif /* ROBOT_ACTION_INCLUDE_ROBOT_ACTION_MYROBOT_ROS_CLIENT_H_ */
