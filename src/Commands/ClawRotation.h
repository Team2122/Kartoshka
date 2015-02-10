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
	float currentAngle;
	enum Direction {
		kBackward, kForward
	};
	Direction dir;

public:
	ClawRotation(YAML::Node config) :
			CommandBase("ClawRotation") {
		newAngle = config["newAngle"].as<float>();
		currentAngle = 0;
	}
	void Initialize() {
		CommandBase::Initialize();
		currentAngle = claw->GetAngle();
		const char* name;
		if (newAngle >= currentAngle) {
			claw->SetRotationDirection(Claw::kForward);
			dir = kForward;
			name = "up";
		} else {
			claw->SetRotationDirection(Claw::kBackward);
			dir = kBackward;
			name = "down";
		}
		log.Info("We are moving %s to %d degrees", name, currentAngle);
	}
	void Execute() {

	}
	bool IsFinished() {
		currentAngle = claw->GetAngle();
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
		claw->SetRotationDirection(Claw::kStopped);
		log.Info("ClawRotation has ended successfully");
	}
	void Interrupted() {
		claw->SetRotationDirection(Claw::kStopped);
		log.Info("ClawRotation was interuppted");
	}
	static std::string GetBaseName() {
		return "ClawRotation";
	}
}
;

}

#endif
