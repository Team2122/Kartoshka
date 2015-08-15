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

#include "Subsystems/Claw.h"
#include "Subsystems/Drive.h"
#include "Subsystems/Claw.h"
#include "Subsystems/Otto.h"
#include "Subsystems/Shuttle.h"
#include "Subsystems/Thief.h"
#include "Subsystems/ToteFeed.h"

namespace tator {

Robot::Robot() :
		log("Robot") {
	scheduler = Scheduler::GetInstance();
	tester = Tester::GetInstance();
	hasRunAuto = false;
	RobotCommand::robot = this;

	claw = nullptr;
	drive = nullptr;
	shuttle = nullptr;
	thief = nullptr;
	toteFeed = nullptr;
	otto = nullptr;
}

Robot::~Robot() {
	delete claw;
	delete drive;
	delete shuttle;
	delete thief;
	delete toteFeed;
	delete otto;

	RobotCommand::robot = nullptr;
}

void Robot::InitSubsystems(YAML::Node subsystems) {
	claw = new Claw(subsystems["Claw"]);
	drive = new Drive(subsystems["Drive"]);
	shuttle = new Shuttle(subsystems["Shuttle"]);
	thief = new Thief(subsystems["Thief"]);
	toteFeed = new ToteFeed(subsystems["ToteFeed"]);
	otto = new Otto(subsystems["Otto"]);
}

void Robot::RobotInit() {
	log.Info("Loading Configs...");
	Config::Load();
	log.Info("Initializing Subsystems...");
	InitSubsystems(Config::subsystems);
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
	otto->GetAutoModeNumber();
	otto->StartGyroCalibration();
}

void Robot::DisabledInit() {
	log.Info("==== DisabledInit ====");
	Kremlin::Get("DriveContinuous")->Cancel();
	tester->Disable();
	USBManager::GetInstance()->Flush();
}

void Robot::AutonomousInit() {
	log.Info("==== AutonomousInit ====");
	if (!hasRunAuto) {
		otto->FinishGyroCalibration();
		otto->StartAutoCommand();
		hasRunAuto = true;
	} else {
		log.Error("Auto does't work if you try to run it twice. Please reboot the robot.");
	}
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
