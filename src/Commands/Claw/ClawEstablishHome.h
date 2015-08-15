#ifndef CLAWESTABLISHHOME_H_
#define CLAWESTABLISHHOME_H_

#include "Robot.h"
#include "Subsystems/Claw.h"
#include <DriverStation.h>

namespace tator {

class ClawEstablishHome: public RobotCommand {
public:
	ClawEstablishHome(std::string name, YAML::Node config) :
			RobotCommand(name) {
	}

	static std::string GetBaseName() {
		return "ClawEstablishHome";
	}

protected:
	void Execute() override {
		robot->claw->Home();
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif
