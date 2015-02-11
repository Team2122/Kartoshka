/**
 * @file SetClaw.h
 * @author Paul Vaughan
 */

#ifndef SETCLAW_H
#define SETCLAW_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include <yaml-cpp/yaml.h>

namespace tator {

class SetClaw: public CommandBase {
private:
	int clawStatus;

public:
	SetClaw(YAML::Node config) :
			CommandBase(GetBaseName()) {
		clawStatus = config["status"].as<int>();
	}
	void Initialize() {
		CommandBase::Initialize();
	}
	void Execute() {
		claw->SetClampStatus((Claw::ClampStatus) clawStatus);
	}
	bool IsFinished() {
		return true;
	}
	void End() {
		CommandBase::End();
	}
	void Interrupted() {
		CommandBase::Interrupted();
	}
	static std::string GetBaseName() {
		return "SetClaw";
	}
};

}

#endif
