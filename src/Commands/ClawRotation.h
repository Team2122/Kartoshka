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
	float newAngle, tolerance;

public:
	ClawRotation(YAML::Node config) :
			CommandBase(GetBaseName()) {
		newAngle = config["status"].as<float>();
		tolerance = config["tolerance"].as<float>();
	}

	void Initialize() {
		CommandBase::Initialize();
		float currentAngle = claw->GetRotationAngle();
		const char* name;
		if (newAngle <= currentAngle) {
			claw->SetRotationSpeed(Claw::RotationSpeed::kForward);
			name = "forward";
		} else {
			claw->SetRotationSpeed(Claw::RotationSpeed::kBackward);
			name = "backward";
		}
		log.Info("We are moving %s to %f degrees", name, newAngle);
	}
	void Execute() {
		float currentAngle = claw->GetRotationAngle();
		if (newAngle <= currentAngle) {
			claw->SetRotationSpeed(Claw::RotationSpeed::kForward);
		} else {
			claw->SetRotationSpeed(Claw::RotationSpeed::kBackward);
		}
	}
	bool IsFinished() {
		float currentAngle = claw->GetRotationAngle();
		return abs(currentAngle - newAngle) <= tolerance;
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
		return "ClawRotation";
	}
};

}

#endif
