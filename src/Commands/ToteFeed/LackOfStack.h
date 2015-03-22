/**
 * @file LackOfStack.h
 * @author Paul Vaughan
 * @date 3/7/2015
 */

#ifndef LACKOFSTACK_H
#define LACKOFSTACK_H

#include "CommandBase.h"
#include "Subsystems/ToteFeed.h"

namespace tator {

class LackOfStack: public CommandBase {
public:
	LackOfStack(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "LackOfStack";
	}

protected:
	void Execute() override {
	}

	bool IsFinished() override {
		return !toteFeed->GetBackSensor();
	}
};

}

#endif
