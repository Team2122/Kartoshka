#ifndef CLAWSMARTROLLERS_H
#define CLAWSMARTROLLERS_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawSmartRollers: public CommandBase {
public:
	ClawSmartRollers(std::string name, YAML::Node config) :
			CommandBase(name) {
		waitTime = config["waitTime"].as<double>();
		speed = config["speed"].as<double>();
		isWaiting = false;
		Requires(claw);
	}

	static std::string GetBaseName() {
		return "ClawSmartRollers";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		claw->SetRollerSpeed(Claw::RollerStatus::kInward, speed);
		isWaiting = false;
		timer.Reset();
	}

	void Execute() override {
		if (claw->HasContainer() && !isWaiting) {
			isWaiting = true;
			claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
			timer.Start();
		}
	}

	bool IsFinished() override {
		return isWaiting && timer.HasPeriodPassed(waitTime);
	}

	void End() override {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped, speed);
		timer.Stop();
		CommandBase::End();
	}

	void Interrupted() override {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped, speed);
		timer.Stop();
		CommandBase::Interrupted();
	}

private:
	double waitTime;
	double speed;
	bool isWaiting;
	Timer timer;
};

}

#endif /* CLAWSMARTROLLERS_H */
