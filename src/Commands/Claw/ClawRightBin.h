/**
 * @file ClawRightBin.h
 * @author Nick Hodes
 * @date April 14, 2015
 */

#ifndef CLAWRIGHTBIN_H_
#define CLAWRIGHTBIN_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawRightBin: public CommandBase {
public:
	ClawRightBin(std::string name, YAML::Node config) :
		CommandBase(name) {
		speed = config["speed"].as<float>();
	}

	static std::string GetBaseName() {
		return "ClawRightBin";
	}

protected:
	void Execute() override {
		claw->SetClampStatus(Claw::ClampStatus::kReleased);
		claw->SetRollerSpeed(Claw::RollerStatus::kInward, speed);
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
	}

	void Interrupted() override {
		claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
	}

private:
	float speed;
};

}

#endif /* TOTEFEED_H_ */
