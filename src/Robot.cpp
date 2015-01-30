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
#include <WPILib.h>

namespace tator {

Robot::Robot() :
	log("Robot"){
	scheduler = Scheduler::GetInstance();
	tester = Tester::GetInstance();
	isTestingAutomated = false;
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
	tester->Interrupted();
}

void Robot::AutonomousInit() {
}

void Robot::TeleopInit() {
	Kremlin::Get("DriveContinuous")->Start();
}

void Robot::TestInit() {
	log.Info("Test mode enabled");
	log.Info("Press A for LiveWindow or B for the automated Tester");
	Joystick* joy = Joystick::GetStickForPort(0);
	while (true) {
		if (joy->GetRawButton(2)) {
			isTestingAutomated = false;
			log.Info("Starting LiveWindow");
			break;
		} else if (joy->GetRawButton(3)) {
			isTestingAutomated = true;
			tester->Initialize();
			break;
		}
	}
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
	if (isTestingAutomated) {
		tester->Execute();
	} else {
		LiveWindow::GetInstance()->Run();
	}
}

} /* namespace tator */

START_ROBOT_CLASS(tator::Robot);
