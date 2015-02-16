/**
 * @file ClawRotationContinuous.h
 * @author Paul Vaughan
 * @date 2/16/2015
 */

#ifndef CLAWROTATIONCONTINUOUS_H
#define CLAWROTATIONCONTINUOUS_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include <yaml-cpp/yaml.h>

namespace tator {

class ClawRotationContinuous: public CommandBase {
protected:
	double tolerance;

public:
	ClawRotationContinuous(std::string name, YAML::Node config) :
			CommandBase(name) {
		tolerance = config["tolerance"].as<double>();
	}

	void Initialize() {
		CommandBase::Initialize();
		claw->SetTargetAngle(claw->GetRotationAngle());
	}

	void Execute() {
		double currentAngle = claw->GetRotationAngle();
		if (abs(currentAngle - claw->GetTargetAngle()) <= tolerance) {
			claw->SetRotationSpeed(Claw::RotationSpeed::kStopped);
		} else if (claw->GetTargetAngle() <= currentAngle) {
			claw->SetRotationSpeed(Claw::RotationSpeed::kForward);
		} else {
			claw->SetRotationSpeed(Claw::RotationSpeed::kBackward);
		}

	}
	bool IsFinished() {
		return false;
	}

	void End() {
		claw->SetRotationSpeed(Claw::RotationSpeed::kStopped);
		CommandBase::End();
	}

	void Interrupted() {
		claw->SetRotationSpeed(Claw::RotationSpeed::kStopped);
		CommandBase::Interrupted();
	}

	static std::string GetBaseName() {
		return "ClawRotationContinuous";
	}
};

}

#endif
