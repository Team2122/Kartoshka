/**
 * @file Robot.cpp
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#include "Robot.h"
#include "Common/Config/Config.h"
#include "Common/Config/Binder.h"
#include "Common/Config/Kremlin.h"
#include "Common/USBManager.h"
#include "CommandBase.h"
#include "Subsystems/Otto.h"

namespace tator {

Robot::Robot() :
		log("Robot") {
	scheduler = Scheduler::GetInstance();
	tester = Tester::GetInstance();
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
	USBManager::GetInstance()->Initialize();
	tester->CreateTests();
	Kremlin::Get("ClawEstablishHome")->Start();
	Kremlin::Get("$ShuttleInit")->Start();
	CommandBase::otto->GetAutoModeNumber();
	CommandBase::otto->StartGyroCalibration();
}

void Robot::DisabledInit() {
	log.Info("==== DisabledInit ====");
	Kremlin::Get("DriveContinuous")->Cancel();
	tester->Disable();
	USBManager::GetInstance()->Flush();
}

void Robot::AutonomousInit() {
	log.Info("==== AutonomousInit ====");
	CommandBase::otto->FinishGyroCalibration();
	CommandBase::otto->StartAutoCommand();
}

void Robot::TeleopInit() {
	log.Info("==== TeleopInit ====");
	Kremlin::Get("DriveContinuous")->Start();
}

void Robot::TestInit() {
	log.Info("==== TestInit ====");
	tester->Initialize();
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
	tester->Run();
}

} /* namespace tator */

START_ROBOT_CLASS(tator::Robot);
