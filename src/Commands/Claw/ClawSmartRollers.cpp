#include "ClawSmartRollers.h"

namespace tator {

COMMAND_CONSTRUCTOR_DEF(ClawSmartRollers) {
	waitTime = config["waitTime"].as<double>();
	isWaiting = false;
	Requires(claw);
}

void ClawSmartRollers::Initialize() {
	CommandBase::Initialize();
	claw->SetRollerSpeed(Claw::RollerStatus::kInward);
	isWaiting = false;
	timer.Reset();
}

void ClawSmartRollers::Execute() {
	if (claw->HasContainer() && !isWaiting) {
		isWaiting = true;
		claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
		timer.Start();
	}
}

bool ClawSmartRollers::IsFinished() {
	return isWaiting && timer.HasPeriodPassed(waitTime);
}

void ClawSmartRollers::End() {
	claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
	timer.Stop();
	CommandBase::End();
}

void ClawSmartRollers::Interrupted() {
	claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
	timer.Stop();
	CommandBase::Interrupted();
}

}
