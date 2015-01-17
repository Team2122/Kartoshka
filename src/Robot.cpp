/**
 * @file Robot.cpp
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#include <Robot.h>

namespace tator {

Robot::Robot() {
	scheduler = Scheduler::GetInstance();
}

Robot::~Robot() {
}

void Robot::RobotInit() {
}

void Robot::DisabledInit() {
}

void Robot::AutonomousInit() {
}

void Robot::TeleopInit() {
}

void Robot::TestInit() {
}

void Robot::DisabledPeriodic() {
}

void Robot::AutonomousPeriodic() {
	scheduler->Run();
}

void Robot::TeleopPeriodic() {
	scheduler->Run();
}

void Robot::TestPeriodic() {
}

} /* namespace tator */

START_ROBOT_CLASS(tator::Robot);
