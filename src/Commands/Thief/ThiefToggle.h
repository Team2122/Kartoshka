/**
 * @file ThiefToggle.h
 * @author Alex Mikhalev
 */

#ifndef THIEFTOGGLE_H
#define THIEFTOGGLE_H

#include "Robot.h"
#include "Subsystems/Thief.h"

namespace tator {

class ThiefToggle: public RobotCommand {
public:
	ThiefToggle(std::string name, YAML::Node config) :
			RobotCommand(name) {
	}

	static std::string GetBaseName() {
		return "ThiefToggle";
	}

protected:
	void Execute() override {
		robot->thief->Toggle();
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif
