/**
 * @file ThiefGrabber.h
 * @author Nick Hodes
 */

#ifndef THIEFGRABBER_H
#define THIEFGRABBER_H

#include "Robot.h"
#include "Subsystems/Thief.h"

namespace tator {

class ThiefGrabber: public RobotCommand {
public:
	ThiefGrabber(std::string name, YAML::Node config) :
			RobotCommand(name) {
		thiefer = (Thief::State) config["status"].as<int>();
	}

	static std::string GetBaseName() {
		return "ThiefGrabber";
	}
protected:
	void Execute() override {
		robot->thief->Set(thiefer);
	}

	bool IsFinished() override {
		return true;
	}
private:
	Thief::State thiefer;
};

}

#endif
