/**
 * @file AddTote.h
 * @author Alex Mikhalev
 * @date 2/14/2015
 */

#ifndef ADDTOTE_H_
#define ADDTOTE_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class AddTote: public CommandBase {
public:
	AddTote(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "AddTote";
	}

protected:
	void Execute() override {
		shuttle->IncrementToteCount(1);
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif
