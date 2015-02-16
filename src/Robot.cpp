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
	testMode = TestMode::livewindow;
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
	Kremlin::Get("ClawEstablishHome")->Start();
}

void Robot::TeleopInit() {
	Kremlin::Get("ClawEstablishHome")->Start();
	Kremlin::Get("DriveContinuous")->Start();
}

void Robot::TestInit() {
	log.Info("Test mode enabled");
	log.Info("Defaulted to LiveWindow");
	log.Info("Press X to switch to the automated Tester");
	testMode = TestMode::livewindow;
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
	Joystick* joy = Joystick::GetStickForPort(0);
	switch (testMode) {
	case TestMode::livewindow:
		LiveWindow::GetInstance()->Run();
		if (joy->GetRawButton(1)) {
			log.Info("Switching to automated testing");
			tester->Initialize();
		}
		break;
	case TestMode::autonomous:
		tester->Execute();
		break;
	}
}

} /* namespace tator */

START_ROBOT_CLASS(tator::Robot);
