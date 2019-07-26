/*
 * myrobot.cpp
 *
 *  Created on: Jul 26, 2019
 *      Author: kukauser
 */

#include "robot_action/myrobot.h"
#include <sstream>
#include <mutex>
#include <iostream>
#include <cmath>

using namespace myrobot;

MyRobot::MyRobot():
    name_("Noname Robot"),
    current_position_(0.0),
    commanded_position_(0.0),
    velocity_(1.0),
    commanding_active_(false),
    feedback_period_seconds_(1),
    step_callback_([](double dummy){}),
    position_reached_callback_([](double dummy){})
{

}

MyRobot::MyRobot(std::string name):
    MyRobot()
{
  name_ = name;
}

std::string MyRobot::GetName()
{
  return name_;
}

void MyRobot::MoveToPosition(double commanded_position)
{
  commanded_position_ = commanded_position;
  if(!commanding_active_)
  {
    commanding_active_ = true;
    simulation_thread_ = std::thread(&MyRobot::SimulateMovement, this);
    simulation_thread_.detach();
  }
}

void MyRobot::SimulateMovement()
{
  const double resolution_ms = 10;
  int32_t tick_count = 0;
  std::stringstream ss;
  std::mutex mu;
  do{
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(resolution_ms)));
    mu.lock();
    double direction = commanded_position_ > current_position_ ? 1 : -1;
    current_position_ += direction*velocity_/1000*resolution_ms;
    if(++tick_count >= feedback_period_seconds_*1000/resolution_ms)
    {
      step_callback_(current_position_);
      tick_count = 0;
    }
    mu.unlock();
  }while(fabs(commanded_position_ - current_position_) >= (velocity_/1000*resolution_ms));
  mu.lock();
  position_reached_callback_(current_position_);
  commanding_active_ = false;
  mu.unlock();
}

void MyRobot::SetStepCallback(std::function<void(double)> callback)
{
  step_callback_ = callback;
}

void MyRobot::SetPositionReachedCallback(std::function<void(double)> callback)
{
  position_reached_callback_ = callback;
}

void MyRobot::SetFeedbackPeriod(int32_t period_seconds)
{
  feedback_period_seconds_ = period_seconds;
}
