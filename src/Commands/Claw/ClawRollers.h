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
		Requires(claw);
	}

	static std::string GetBaseName() {
		return "ClawRollers";
	}

protected:
	void Execute() override {
		claw->SetRollerSpeed(rollerStatus);
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
		CommandBase::End();
	}

	void Interrupted() override {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
		CommandBase::Interrupted();
	}

private:
	Claw::RollerStatus rollerStatus;
};

}

#endif /* CLAWROLLERS_H */
