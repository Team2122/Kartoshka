/**
 * @file ClawRotationContinuous.h
 * @author Paul Vaughan
 * @date 2/16/2015
 */

#ifndef CLAWROTATIONCONTINUOUS_H
#define CLAWROTATIONCONTINUOUS_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawRotationContinuous: public CommandBase {
public:
	ClawRotationContinuous(std::string name, YAML::Node config) :
			CommandBase(name) {
		tolerance = config["tolerance"].as<double>();
		YAML::Node speeds = config["speeds"];
		moveSpeed = speeds["move"].as<double>();
		holdSpeed = speeds["hold"].as<double>();
		angles = config["angles"].as<std::vector<double>>();
	}

	static std::string GetBaseName() {
		return "ClawRotationContinuous";
	}

protected:
	void Execute() override {
		double currentAngle = claw->GetRotationAngle(); // current angle of the claw
		Claw::ClawAngle clawAngle = claw->GetTargetAngle(); // the number of the target angle
		double targetAngle = angles[(int) clawAngle]; // the actual target angle
		double difference = abs(targetAngle - currentAngle);
		bool hold = difference <= tolerance; // if we are within our tolerance
		double speed;
		if (clawAngle == Claw::ClawAngle::kPick) { // towards pick: negative
			speed = -1;
		} else if (clawAngle == Claw::ClawAngle::kPlatform) { // towards platform: positive
			speed = 1;
		}
		if (hold) {
			speed *= holdSpeed;
			claw->SetRotationFinished(true); // if we are applying hold power, tell ClawRotation to finish
		} else {
			speed *= moveSpeed;
			claw->SetRotationFinished(false);
		}
		claw->SetRotationSpeed(speed);
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		claw->SetRotationSpeed(0);
		CommandBase::End();
	}

	void Interrupted() override {
		claw->SetRotationSpeed(0);
		CommandBase::Interrupted();
	}

private:
	double tolerance, moveSpeed, holdSpeed;
	std::vector<double> angles; // holds the two angles the claw can be at
};

}

#endif
