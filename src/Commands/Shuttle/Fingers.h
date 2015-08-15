/**
 * @file Fingers.h
 * @author Daniele Moro
 * @date 2/11/2015
 */

#ifndef FINGERS_H_
#define FINGERS_H_

#include "Robot.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class Fingers: public RobotCommand {
public:
	Fingers(std::string name, YAML::Node config) :
			RobotCommand(name) {
		state = (Shuttle::FingersState) config["state"].as<int>();
	}

	static std::string GetBaseName() {
		return "Fingers";
	}

protected:
	void Execute() override {
		robot->shuttle->SetFingersPiston(state);
	}

	bool IsFinished() override {
		return true;
	}

private:
	Shuttle::FingersState state;
};

}

#endif
