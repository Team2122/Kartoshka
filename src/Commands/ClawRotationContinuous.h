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
	double tolerance, holdAngle;
	std::map<std::string, double> angles;

public:
	ClawRotationContinuous(std::string name, YAML::Node config) :
			CommandBase(name) {
		tolerance = config["tolerance"].as<double>();
		angles = config["angles"].as<std::map<std::string, double>>();
	}

	void Initialize() {
		CommandBase::Initialize();
		claw->SetTargetAngle("Hold");
		holdAngle = claw->GetRotationAngle();
	}

	void Execute() {
		double currentAngle = claw->GetRotationAngle();
		double targetAngle;
		if (claw->GetTargetAngle() == "Hold") {
			targetAngle = holdAngle;
		} else {
			targetAngle = angles[claw->GetTargetAngle()];
		}
		if (abs(currentAngle - targetAngle) <= tolerance) {
			if (claw->GetTargetAngle() == "Pick") {
				claw->SetRotationSpeed(Claw::RotationSpeed::kHoldPick);
			} else {
				claw->SetRotationSpeed(Claw::RotationSpeed::kStopped);
			}
			claw->SetRotationFinished(true);
		} else if (targetAngle <= currentAngle) {
			claw->SetRotationSpeed(Claw::RotationSpeed::kForward);
			claw->SetRotationFinished(false);
		} else {
			claw->SetRotationSpeed(Claw::RotationSpeed::kBackward);
			claw->SetRotationFinished(false);
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
