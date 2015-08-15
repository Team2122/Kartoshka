#ifndef CLAWSMARTROLLERS_H
#define CLAWSMARTROLLERS_H

#include "Robot.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawSmartRollers: public RobotCommand {
public:
	ClawSmartRollers(std::string name, YAML::Node config) :
			RobotCommand(name) {
		waitTime = config["waitTime"].as<double>();
		isWaiting = false;
		Requires(robot->claw);
	}

	static std::string GetBaseName() {
		return "ClawSmartRollers";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		robot->claw->SetRollerSpeed(Claw::RollerStatus::kInward);
		isWaiting = false;
		timer.Reset();
	}

	void Execute() override {
		if (robot->claw->HasContainer() && !isWaiting) {
			isWaiting = true;
			robot->claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
			timer.Start();
		}
	}

	bool IsFinished() override {
		return isWaiting && timer.HasPeriodPassed(waitTime);
	}

	void End() override {
		robot->claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
		timer.Stop();
		RobotCommand::End();
	}

	void Interrupted() override {
		robot->claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
		timer.Stop();
		RobotCommand::Interrupted();
	}

private:
	double waitTime;
	bool isWaiting;
	Timer timer;
};

}

#endif /* CLAWSMARTROLLERS_H */
