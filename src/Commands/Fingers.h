/**
 * @file Fingers.h
 * @author Daniele Moro
 * @date 2/11/2015
 */

#ifndef FINGERS_H_
#define FINGERS_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class Fingers: public CommandBase {
public:
	Fingers(std::string name, YAML::Node config) :
			CommandBase(name) {
		state = (Shuttle::FingersState) config["state"].as<int>();
	}

	static std::string GetBaseName() {
		return "Fingers";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
	}

	virtual void Execute() {
		shuttle->SetFingersPiston(state);
	}

	virtual bool IsFinished() {
		return true;
	}

	virtual void End() {
	}

	virtual void Interrupted() {
	}

protected:
	Shuttle::FingersState state;
};

}

#endif
