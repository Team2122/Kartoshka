/**
 * @file ResetAngle.h
 * @author Alex Mikhalev
 */

#ifndef RESETANGLE_H_
#define RESETANGLE_H_

#include "Robot.h"

namespace tator {

class ResetAngle: public RobotCommand {
public:
	ResetAngle(std::string name, YAML::Node config) :
			RobotCommand(name) {
	}

	static std::string GetBaseName() {
		return "ResetAngle";
	}

protected:
	void Execute() override {
		robot->otto->ResetAngle();
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif
