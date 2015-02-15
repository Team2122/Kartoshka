#ifndef CLAWSMARTROLLERS_H
#define CLAWSMARTROLLERS_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include <yaml-cpp/yaml.h>

namespace tator {

class ClawSmartRollers: public CommandBase {
public:
	ClawSmartRollers(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	void Initialize() {
		CommandBase::Initialize();
		claw->SetRollerSpeed(Claw::RollerStatus::kInward);
	}

	void Execute() {
	}

	bool IsFinished() {
		return claw->HasContainer();
	}

	void End() {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
		CommandBase::End();
	}

	void Interrupted() {
		claw->SetRollerSpeed(Claw::RollerStatus::kStopped);
		CommandBase::Interrupted();
	}

	static std::string GetBaseName() {
		return "ClawSmartRollers";
	}
};
}

#endif /* CLAWROLLERS_H */
