/**
 * @file Robot.cpp
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#include <Robot.h>
#include "Common/Config.h"
#include "Common/Binder.h"
#include "Common/Kremlin.h"
#include "CommandBase.h"

namespace tator {

Robot::Robot() {
	scheduler = Scheduler::GetInstance();
}

Robot::~Robot() {
}

void Robot::RobotInit() {
	Config::Load();
	Kremlin::CreateCommands();
	Binder::BindAll();
	CommandBase::InitSubsystems(Config::subsystems);
	Config::Delete();
}

void Robot::DisabledInit() {
	Kremlin::Get("DriveContinuous")->Cancel();
}

void Robot::AutonomousInit() {
}

void Robot::TeleopInit() {
	Kremlin::Get("DriveContinuous")->Start();
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
	LiveWindow::GetInstance()->Run();
}

} /* namespace tator */

START_ROBOT_CLASS(tator::Robot);
