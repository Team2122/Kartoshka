/**
 * @file ClawClamp.h
 * @author Paul Vaughan
 */

#ifndef CLAWCLAMP_H
#define CLAWCLAMP_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include <yaml-cpp/yaml.h>

namespace tator {

class ClawClamp: public CommandBase {
private:
	Claw::ClampStatus clampStatus;

public:
	ClawClamp(YAML::Node config) :
			CommandBase(GetBaseName()) {
		clampStatus = (Claw::ClampStatus) config["status"].as<int>();
	}

	void Initialize() {
		CommandBase::Initialize();
	}

	void Execute() {
		claw->SetClampStatus(clampStatus);
	}

	bool IsFinished() {
		return true;
	}

	void End() {
	}

	void Interrupted() {
	}

	static std::string GetBaseName() {
		return "ClawClamp";
	}
};

}

#endif
