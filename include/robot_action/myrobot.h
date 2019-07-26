/*
 * myrobot.h
 *
 *  Created on: Jul 25, 2019
 *      Author: kukauser
 */

#ifndef ROBOT_ACTION_INCLUDE_ROBOT_ACTION_MYROBOT_H_
#define ROBOT_ACTION_INCLUDE_ROBOT_ACTION_MYROBOT_H_

#include <string>
#include <functional>
#include <chrono>
#include <thread>

namespace myrobot{

class MyRobot
{
public:
  MyRobot();
  MyRobot(std::string name);

  void MoveToPosition(double commanded_position);
  void SetPositionReachedCallback(std::function<void(double)> callback);
  void SetStepCallback(std::function<void(double)> callback);
  void SetFeedbackPeriod(int32_t period_seconds);

  std::string GetName();
private:
  void SimulateMovement();

  std::string name_;
  double current_position_;
  double commanded_position_;
  double velocity_;
  bool commanding_active_;
  int32_t feedback_period_seconds_;
  std::thread simulation_thread_;
  std::function<void(double)> step_callback_;
  std::function<void(double)> position_reached_callback_;

};//class MyRobot

} // namespace myrobot

#endif /* ROBOT_ACTION_INCLUDE_ROBOT_ACTION_MYROBOT_H_ */
