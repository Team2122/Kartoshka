/**
 * @file LackOfStack.h
 * @author Paul Vaughan
 * @date 3/7/2015
 */

#ifndef LACKOFSTACK_H
#define LACKOFSTACK_H

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class LackOfStack: public CommandBase {
public:
	LackOfStack(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "LackOfStack";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
	}

	virtual void Execute() {
	}

	virtual bool IsFinished() {
		return !shuttle->IsTotePresent();
	}

	virtual void End() {
	}

	virtual void Interrupted() {
	}

protected:
};

}

#endif
