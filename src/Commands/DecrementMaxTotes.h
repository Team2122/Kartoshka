/**
 * @file DecrementMaxTotes.h
 * @author Alex Mikhalev
 * @date 3/8/2015
 */

#ifndef DECREMENTMAXTOTES_H_
#define DECREMENTMAXTOTES_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class DecrementMaxTotes: public CommandBase {
public:
	DecrementMaxTotes(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "DecrementMaxTotes";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
	}

	virtual void Execute() {
		shuttle->DecrementMaxToteCount();
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
