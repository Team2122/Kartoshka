/**
 * @file ClawClamp.h
 * @author Paul Vaughan
 */

#ifndef CLAWCLAMP_H
#define CLAWCLAMP_H

#include "Robot.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawClamp: public RobotCommand {
public:
	ClawClamp(std::string name, YAML::Node config) :
			RobotCommand(name) {
		clampStatus = (Claw::ClampStatus) config["status"].as<int>();
	}

	static std::string GetBaseName() {
		return "ClawClamp";
	}

protected:
	void Execute() override {
		robot->claw->SetClampStatus(clampStatus);
	}

	bool IsFinished() override {
		return true;
	}

private:
	Claw::ClampStatus clampStatus;
};

}

#endif
