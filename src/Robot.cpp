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

Robot::Robot() :
	log("Robot"){
	scheduler = Scheduler::GetInstance();
}

Robot::~Robot() {
}

void Robot::RobotInit() {
	log.Info("Loading Configs...");
	Config::Load();
	log.Info("Creating Commands...");
	Kremlin::CreateCommands();
	log.Info("Binding all things...");
	Binder::BindAll();
	log.Info("Initializing Subsystems...");
	CommandBase::InitSubsystems(Config::subsystems);
	log.Info("Deleting Configs...");
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
