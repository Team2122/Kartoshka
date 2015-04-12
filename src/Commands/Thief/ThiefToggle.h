/**
 * @file ThiefToggle.h
 * @author Alex Mikhalev
 */

#ifndef THIEFTOGGLE_H
#define THIEFTOGGLE_H

#include "CommandBase.h"
#include "Subsystems/Thief.h"

namespace tator {

class ThiefToggle: public CommandBase {
public:
	ThiefToggle(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "ThiefToggle";
	}

protected:
	void Execute() override {
		thief->Toggle();
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif
