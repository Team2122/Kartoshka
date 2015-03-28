#ifndef CLAWROLLERS_H
#define CLAWROLLERS_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawRollers: public CommandBase {
public:
	ClawRollers(std::string name, YAML::Node config) :
			CommandBase(name) {
		rollerStatus = (Claw::RollerStatus) config["status"].as<int>();
		speed = config["speed"].as<double>();
		Requires(claw);
	}

	static std::string GetBaseName() {
		return "ClawRollers";
	}

protected:
	void Execute() override {
		claw->SetRollerSpeed(rollerStatus, speed);
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped, speed);
		CommandBase::End();
	}

	void Interrupted() override {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped, speed);
		CommandBase::Interrupted();
	}

private:
	Claw::RollerStatus rollerStatus;
	double speed;
};

}

#endif /* CLAWROLLERS_H */
