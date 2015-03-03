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
	double tolerance, moveSpeed, holdSpeed;
	std::vector<double> angles;

public:
	ClawRotationContinuous(std::string name, YAML::Node config) :
			CommandBase(name) {
		tolerance = config["tolerance"].as<double>();
		moveSpeed = config["speeds"]["move"].as<double>();
		holdSpeed = config["speeds"]["hold"].as<double>();
		angles = config["angles"].as<std::vector<double>>();
	}

	void Initialize() {
		CommandBase::Initialize();
	}

	void Execute() {
		double currentAngle = claw->GetRotationAngle();
		Claw::ClawAngle clawAngle = claw->GetTargetAngle();
		double targetAngle = angles[(int) clawAngle];
		double difference = abs(targetAngle - currentAngle);
		bool hold = difference <= tolerance;
		double speed;
		if (clawAngle == Claw::ClawAngle::kPick) {
			speed = -1;
		} else if (clawAngle == Claw::ClawAngle::kPlatform) {
			speed = 1;
		}
		if (hold) {
			speed *= holdSpeed;
			claw->SetRotationFinished(true);
		} else {
			speed *= moveSpeed;
			claw->SetRotationFinished(false);
		}
		claw->SetRotationSpeed(speed);
	}
	bool IsFinished() {
		return false;
	}

	void End() {
		claw->SetRotationSpeed(0);
		CommandBase::End();
	}

	void Interrupted() {
		claw->SetRotationSpeed(0);
		CommandBase::Interrupted();
	}

	static std::string GetBaseName() {
		return "ClawRotationContinuous";
	}
};

}

#endif
