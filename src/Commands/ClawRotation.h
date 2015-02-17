/**
 * @file ClawRotation.h
 * @author Paul Vaughan
 */

#ifndef CLAWROTATION_H
#define CLAWROTATION_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include <yaml-cpp/yaml.h>

namespace tator {

class ClawRotation: public CommandBase {
private:
	double targetAngle;

public:
	ClawRotation(std::string name, YAML::Node config) :
			CommandBase(name) {
		targetAngle = config["angle"].as<double>();
	}

	void Initialize() {
		CommandBase::Initialize();
		claw->SetTargetAngle(targetAngle);
	}
	void Execute() {
	}

	bool IsFinished() {
		return claw->GetRotationSpeed() == Claw::RotationSpeed::kStopped;
	}

	void End() {
		CommandBase::End();
	}

	void Interrupted() {
		CommandBase::Interrupted();
	}

	static std::string GetBaseName() {
		return "ClawRotation";
	}
};

}

#endif
