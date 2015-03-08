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
#include "Subsystems/Otto.h"
#include <WPILib.h>

namespace tator {

Robot::Robot() :
		log("Robot") {
	scheduler = Scheduler::GetInstance();
	tester = Tester::GetInstance();
	manualTester = ManualTester::GetInstance();
	testMode = TestMode::manual;
}

Robot::~Robot() {
}

void Robot::RobotInit() {
	log.Info("Loading Configs...");
	Config::Load();
	log.Info("Initializing Subsystems...");
	CommandBase::InitSubsystems(Config::subsystems);
	log.Info("Creating Commands...");
	Kremlin::CreateCommands();
	log.Info("Binding all things...");
	Binder::BindAll();
	log.Info("Deleting Configs...");
	Config::Delete();
	Kremlin::Get("ClawEstablishHome")->Start();
	Kremlin::Get("$ShuttleInit")->Start();
	Kremlin::Get("ClawRotationContinuous")->Start();
	CameraServer::GetInstance()->StartAutomaticCapture("cam2");
}

void Robot::DisabledInit() {
	log.Info("==== DisabledInit ====");
	Kremlin::Get("DriveContinuous")->Cancel();
	tester->Interrupted();
}

void Robot::AutonomousInit() {
	log.Info("==== AutonomousInit ====");
	CommandBase::otto->StartAutoCommand();
}

void Robot::TeleopInit() {
	log.Info("==== TeleopInit ====");
	Kremlin::Get("DriveContinuous")->Start();
}

void Robot::TestInit() {
	log.Info("==== TestInit ====");
	log.Info("Defaulted to the ManualTester");
	log.Info("Press START (10) to switch to the automated Tester");
	testMode = TestMode::manual;
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
	case TestMode::manual:
		manualTester->Execute();
		if (joy->GetRawButton(10)) {
			log.Info("Switching to automated testing");
			manualTester->StopTesting();
			tester->Initialize();
			testMode = TestMode::autonomous;
		}
		break;
	case TestMode::autonomous:
		tester->Execute();
		break;
	}
}

} /* namespace tator */

START_ROBOT_CLASS(tator::Robot);
