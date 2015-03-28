/**
 * @file ThiefGrabber.h
 * @author Nick Hodes
 */

#ifndef THIEFGRABBER_H
#define THIEFGRABBER_H

#include "CommandBase.h"
#include "Subsystems/Thief.h"

namespace tator {

class ThiefGrabber: public CommandBase {
public:
	ThiefGrabber(std::string name, YAML::Node config) :
			CommandBase(name) {
		thiefer = (Thief::State) config["status"].as<int>();
	}

	static std::string GetBaseName(){
		return "ThiefGrabber";
	}
protected:
	void Execute() override {
		thief->Set(thiefer);
	}

	bool IsFinished() override {
		return true;
	}
private:
	Thief::State thiefer;
};

}

#endif
