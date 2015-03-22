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

protected:
	void Execute() override {
		shuttle->SetFingersPiston(state);
	}

	bool IsFinished() override {
		return true;
	}

private:
	Shuttle::FingersState state;
};

}

#endif
