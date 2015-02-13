/**
 * @file SmoothClawClamp.h
 * @author Paul Vaughan
 * @date 2/12/2015
 */

#ifndef SMOOTHCLAWCLAMP_H
#define SMOOTHCLAWCLAMP_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class SmoothClawClamp: public CommandBase {
public:
	SmoothClawClamp(YAML::Node config) :
			CommandBase(GetBaseName()) {
	}

	virtual void Initialize() {
		CommandBase::Initialize();
	}

	virtual void Execute() {
		claw->ClampClaw();
	}

	virtual bool IsFinished() {
		return true;
	}

	virtual void End() {
	}

	virtual void Interrupted() {
	}

	static std::string GetBaseName() {
		return "SmoothClawClamp";
	}
};

}

#endif
