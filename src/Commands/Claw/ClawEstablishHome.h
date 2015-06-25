#ifndef CLAWESTABLISHHOME_H_
#define CLAWESTABLISHHOME_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include <DriverStation.h>

namespace tator {

class ClawEstablishHome: public CommandBase {
public:
	ClawEstablishHome(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "ClawEstablishHome";
	}

protected:
	void Execute() override {
		claw->Home();
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif
