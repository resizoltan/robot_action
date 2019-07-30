/*
 * myrobot_ros_client.cpp
 *
 *  Created on: Jul 30, 2019
 *      Author: kukauser
 */

#include "robot_action/myrobot_ros_client.h"

using namespace robot_action;

MyRobotROSClient::MyRobotROSClient(ros::NodeHandle nh, MyRobot& myRobot):
    myRobot_(myRobot),
    action_server_(nh, "move_robot", true)
{
  myRobot_.SetStepCallback([this](double pos){this->FeedbackCallback(pos);});
  myRobot_.SetPositionReachedCallback([this](double pos){this->ResultCallback(pos);});
  action_server_.registerGoalCallback([this](){this->NewGoalCallback();});
}

void MyRobotROSClient::NewGoalCallback()
{
  double command_position = action_server_.acceptNewGoal()->command_position;
  myRobot_.MoveToPosition(command_position);
}

void MyRobotROSClient::FeedbackCallback(double current_position)
{
  feedback_.current_position = current_position;
  action_server_.publishFeedback(feedback_);
}

void MyRobotROSClient::ResultCallback(double end_position)
{
  result_.end_position = end_position;
  action_server_.setSucceeded(result_);
}
