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
	float newAngle;

	enum Direction {
		kBackward, kForward
	};

	Direction dir;

public:
	ClawRotation(YAML::Node config) :
			CommandBase(GetBaseName()) {
		newAngle = config["status"].as<float>();
	}

	void Initialize() {
		CommandBase::Initialize();
		float currentAngle = claw->GetRotationAngle();
		const char* name;
		if (newAngle >= currentAngle) {
			claw->SetRotationSpeed(Claw::RotationSpeed::kForward);
			dir = kForward;
			name = "up";
		} else {
			claw->SetRotationSpeed(Claw::RotationSpeed::kBackward);
			dir = kBackward;
			name = "down";
		}
		log.Info("We are moving %s to %d degrees", name, currentAngle);
	}
	void Execute() {

	}
	bool IsFinished() {
		float currentAngle = claw->GetRotationAngle();
		switch (dir) {
		case kForward:
			return currentAngle >= newAngle;
		case kBackward:
			return currentAngle <= newAngle;
		default:
			return true;
		}
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
