/*
 * myrobot_test.cpp
 *
 *  Created on: Jul 26, 2019
 *      Author: kukauser
 */

#include "robot_action/myrobot.h"
#include <gtest/gtest.h>
#include <queue>
#include <math.h>

using namespace robot_action;

class MyRobotInterface
{
public:
  MyRobotInterface(MyRobot& myRobot) :
      myRobot_(myRobot), end_position_(0)
  {
    myRobot.SetStepCallback([this](double cp)
    { StepCallback(cp);});
    myRobot.SetPositionReachedCallback([this](double cp)
    { PositionReachedCallback(cp);});
  }

  void StepCallback(double current_position)
  {
    step_positions_.push(current_position);
    //std::cout << current_position << std::endl;
  }

  void PositionReachedCallback(double current_position)
  {
    end_position_ = current_position;
    //std::cout << current_position << std::endl;
  }

  std::queue<double> GetStepPositions()
  {
    return step_positions_;
  }

  double GetEndPosition()
  {
    return end_position_;
  }

  void ResetInterface()
  {
    std::queue<double> empty;
    std::swap(step_positions_, empty);
    end_position_ = 0;
  }

private:
  MyRobot &myRobot_;
  std::queue<double> step_positions_;
  double end_position_;
};

TEST(MyRobot, DISABLED_ReachesPosition) // disabled, because test takes too long. TODO : find better solution
{
  MyRobot myRobot;
  MyRobotInterface myRobotInterface(myRobot);
  double eps = 0.1;

  double ep_cmd = 5;
  myRobot.MoveToPosition(ep_cmd);
  std::this_thread::sleep_for(std::chrono::seconds(6));
  double ep_real = myRobotInterface.GetEndPosition();
  EXPECT_TRUE(fabs(ep_real - ep_cmd) < eps);
  //std::cout << fabs(ep_real-ep_cmd) << std::endl;

  ep_cmd = 3.4;
  myRobot.MoveToPosition(ep_cmd);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  ep_real = myRobotInterface.GetEndPosition();
  EXPECT_TRUE(fabs(ep_real - ep_cmd) < eps);
  //std::cout << fabs(ep_real-ep_cmd) << std::endl;

  ep_cmd = -1.34517;
  myRobot.MoveToPosition(ep_cmd);
  std::this_thread::sleep_for(std::chrono::seconds(6));
  ep_real = myRobotInterface.GetEndPosition();
  EXPECT_TRUE(fabs(ep_real - ep_cmd) < eps);
  //lstd::cout << fabs(ep_real-ep_cmd) << std::endl;

}

TEST(MyRobot, StepPositions)
{
  MyRobot myRobot;
  MyRobotInterface myRobotInterface(myRobot);
  double eps = 0.1;

  double ep_cmd = 5;
  std::queue<double> steps_expected;
  steps_expected.push(1);
  steps_expected.push(2);
  steps_expected.push(3);
  steps_expected.push(4);
  steps_expected.push(5);
  myRobot.MoveToPosition(ep_cmd);
  std::this_thread::sleep_for(std::chrono::seconds(6));
  std::queue<double> steps = myRobotInterface.GetStepPositions();
  EXPECT_TRUE(fabs(steps.front() - steps_expected.front()) < eps);
  steps.pop();
  steps_expected.pop();
  EXPECT_TRUE(fabs(steps.front() - steps_expected.front()) < eps);
  steps.pop();
  steps_expected.pop();
  EXPECT_TRUE(fabs(steps.front() - steps_expected.front()) < eps);
  steps.pop();
  steps_expected.pop();
  EXPECT_TRUE(fabs(steps.front() - steps_expected.front()) < eps);
  steps.pop();
  steps_expected.pop();
  EXPECT_TRUE(fabs(steps.front() - steps_expected.front()) < eps);
  steps.pop();
  steps_expected.pop();
  EXPECT_EQ(steps.size(), 0);


  myRobotInterface.ResetInterface();
  ep_cmd = 2.4;
  steps_expected.push(4);
  steps_expected.push(3);
  myRobot.MoveToPosition(ep_cmd);
  std::this_thread::sleep_for(std::chrono::seconds(3));
  steps = myRobotInterface.GetStepPositions();
  EXPECT_TRUE(fabs(steps.front() - steps_expected.front()) < eps);
  steps.pop();
  steps_expected.pop();
  EXPECT_TRUE(fabs(steps.front() - steps_expected.front()) < eps);
  steps.pop();
  steps_expected.pop();
  EXPECT_EQ(steps.size(), 0);

}

TEST(MyRobot, InterruptMotion)
{
  MyRobot myRobot;
  MyRobotInterface myRobotInterface(myRobot);
  double eps = 0.1;

  double ep_cmd1 = 5;
  double ep_cmd2 = -1;
  myRobot.MoveToPosition(ep_cmd1);
  std::this_thread::sleep_for(std::chrono::seconds(3));
  auto steps = myRobotInterface.GetStepPositions();
  myRobot.MoveToPosition(ep_cmd2);
  std::this_thread::sleep_for(std::chrono::seconds(5));
  auto ep_real = myRobotInterface.GetEndPosition();
  EXPECT_TRUE(fabs(ep_real - ep_cmd2) < eps);

}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
