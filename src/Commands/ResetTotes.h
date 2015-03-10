/**
 * @file ResetTotes.h
 * @author Alex Mikhalev
 * @date 2/14/2015
 */

#ifndef RESETTOTES_H_
#define RESETTOTES_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ResetTotes: public CommandBase {
public:
	ResetTotes(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "ResetTotes";
	}

private:
	void Execute() override {
		shuttle->ResetToteCount();
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif
