/**
 * @file SmoothClawRelease.h
 * @author Paul Vaughan
 * @date 2/12/2015
 */

#ifndef SMOOTHCLAWRELEASE_H
#define SMOOTHCLAWRELEASE_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class SmoothClawRelease: public CommandBase {
public:
	SmoothClawRelease(YAML::Node config) :
			CommandBase(GetBaseName()) {
	}

	virtual void Initialize() {
		CommandBase::Initialize();
	}

	virtual void Execute() {
		claw->ReleaseClaw();
	}

	virtual bool IsFinished() {
		return true;
	}

	virtual void End() {
	}

	virtual void Interrupted() {
	}

	static std::string GetBaseName() {
		return "SmoothClawRelease";
	}
};

}

#endif
