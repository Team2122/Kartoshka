/**
 * @file ClawToAngle.cpp
 */

#include "ClawToAngle.h"

namespace tator {

COMMAND_CONSTRUCTOR_DEF(ClawToAngle) {
	Requires(claw);

	angle = claw->AngleFromName(config["angle"].as<std::string>());
	speed = config["speed"].as<double>();
	holdSpeed = config["holdSpeed"].as<double>();

	YAML::Node ramp = config["ramp"];
	rampDistance = ramp["distance"].as<double>();
	rampFactor = ramp["factor"].as<double>();

	minimumHeight = config["minimumHeight"].as<double>();
}

void ClawToAngle::Initialize() {
	CommandBase::Initialize();
	// If we're above the platform
	// and not already at the angle
	if (claw->GetLiftEncoder() < minimumHeight && !claw->IsAtAngle(angle)) {
		log.Error("We cannot rotate if the height of the claw is less "
				"than %f", minimumHeight);
		return this->Cancel();
	}
}

void ClawToAngle::Execute() {
	double distance = claw->GetDegreesFromAngle(angle);
	double multiplier = 1;
	if (distance <= rampDistance) {
		multiplier = rampFactor;
	}
	claw->SetRotationSpeed(speed * multiplier);
}

bool ClawToAngle::IsFinished() {
	return claw->IsAtAngle(angle);
}

void ClawToAngle::End() {
	CommandBase::End();
	claw->SetRotationSpeed(holdSpeed);
}

void ClawToAngle::Interrupted() {
	CommandBase::Interrupted();
	claw->SetRotationSpeed(0);
}

}

