/**
 * @file ResetMaxTotes.h
 * @author Alex Mikhalev
 * @date 3/8/2015
 */

#ifndef RESETMAXTOTES_H_
#define RESETMAXTOTES_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ResetMaxTotes: public CommandBase {
public:
	ResetMaxTotes(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "ResetMaxTotes";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
	}

	virtual void Execute() {
		shuttle->ResetMaxToteCount();
		log.Info("Will now make a %d high stack", shuttle->GetMaxToteCount() + 2);
	}

	virtual bool IsFinished() {
		return true;
	}

	virtual void End() {
	}

	virtual void Interrupted() {
	}

protected:
};

}

#endif
