#ifndef CLAWROLLERS_H
#define CLAWROLLERS_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include <yaml-cpp/yaml.h>

namespace tator {

class ClawRollers: public CommandBase {
protected:
	Claw::RollerStatus rollerStatus;
public:
	ClawRollers(YAML::Node config) :
			CommandBase(GetBaseName()) {
		rollerStatus = (Claw::RollerStatus) config["status"].as<int>();
	}

	void Initialize() {
		CommandBase::Initialize();
	}

	void Execute() {
		claw->SetRollerSpeed(rollerStatus);
	}

	bool IsFinished() {
		return true;
	}

	void End() {

	}

	void Interrupted() {

	}

	static std::string GetBaseName() {
		return "ClawRollers";
	}
};
}

#endif /* CLAWROLLERS_H */
