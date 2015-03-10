/**
 * @file ClawRotation.h
 * @author Paul Vaughan
 */

#ifndef CLAWROTATION_H
#define CLAWROTATION_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawRotation: public CommandBase {
public:
	ClawRotation(std::string name, YAML::Node config) :
			CommandBase(name) {
		targetAngle = (Claw::ClawAngle) config["targetAngle"].as<int>();
		Requires(claw);
	}

	static std::string GetBaseName() {
		return "ClawRotation";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		claw->SetTargetAngle(targetAngle);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return claw->IsRotationFinished();
	}

private:
	Claw::ClawAngle targetAngle;
};

}

#endif
