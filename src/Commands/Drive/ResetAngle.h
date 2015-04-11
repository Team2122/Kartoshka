/**
 * @file ResetAngle.h
 * @author Alex Mikhalev
 */

#ifndef RESETANGLE_H_
#define RESETANGLE_H_

#include "CommandBase.h"

namespace tator {

class ResetAngle: public CommandBase {
public:
	ResetAngle(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "ResetAngle";
	}

protected:
	void Execute() override {
		otto->ResetAngle();
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif
