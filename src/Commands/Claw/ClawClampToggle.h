/**
 * @file ClawClampToggle.h
 * @author Alex Mikhalev
 * @date 3/8/2015
 */

#ifndef CLAWCLAMPTOGGLE_H_
#define CLAWCLAMPTOGGLE_H_

#include "Robot.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawClampToggle: public RobotCommand {
public:
	ClawClampToggle(std::string name, YAML::Node config) :
			RobotCommand(name) {

	}

	static std::string GetBaseName() {
		return "ClawClampToggle";
	}

protected:
	void Execute() override {
		if (robot->claw->GetClampStatus() == Claw::ClampStatus::kDeathGrip) {
			log.Info("Releasing claw clamp");
			robot->claw->SetClampStatus(Claw::ClampStatus::kReleased);
		} else {
			log.Info("Death gripping the claw");
			robot->claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
		}
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif /* CLAWCLAMPTOGGLE_H_ */
