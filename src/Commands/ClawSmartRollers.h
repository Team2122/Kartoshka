#ifndef CLAWSMARTROLLERS_H
#define CLAWSMARTROLLERS_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawSmartRollers: public CommandBase {
public:
	ClawSmartRollers(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "ClawSmartRollers";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		claw->SetRollerSpeed(Claw::RollerStatus::kInward);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return claw->HasContainer();
	}

	void End() override {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
		CommandBase::End();
	}

	void Interrupted() override {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
		CommandBase::Interrupted();
	}
};

}

#endif /* CLAWSMARTROLLERS_H */
