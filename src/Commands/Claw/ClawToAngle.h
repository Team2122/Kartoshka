/**
 * @file ClawToAngle.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef CLAWTOANGLE_H_
#define CLAWTOANGLE_H_

#include "Robot.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawToAngle: public RobotCommand {
public:
	ClawToAngle(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->claw);

		angle = robot->claw->AngleFromName(config["angle"].as<std::string>());
		speed = config["speed"].as<double>();
		holdSpeed = config["holdSpeed"].as<double>();

		YAML::Node ramp = config["ramp"];
		rampDistance = ramp["distance"].as<double>();
		rampFactor = ramp["factor"].as<double>();

		minimumHeight = config["minimumHeight"].as<double>();
	}

	static std::string GetBaseName() {
		return "ClawToAngle";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		// If we're above the platform
		// and not already at the angle
		if (robot->claw->GetLiftEncoder() < minimumHeight
				&& !robot->claw->IsAtAngle(angle)) {
			log.Error("We cannot rotate if the height of the claw is less "
					"than %f", minimumHeight);
			return this->Cancel();
		}
	}

	void Execute() override {
		double distance = robot->claw->GetDegreesFromAngle(angle);
		double multiplier = 1;
		if (distance <= rampDistance) {
			multiplier = rampFactor;
		}
		robot->claw->SetRotationSpeed(speed * multiplier);
	}

	bool IsFinished() override {
		return robot->claw->IsAtAngle(angle);
	}

	void End() override {
		RobotCommand::End();
		robot->claw->SetRotationSpeed(holdSpeed);
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		robot->claw->SetRotationSpeed(0);
	}

private:
	Claw::ClawAngle angle;
	double speed, holdSpeed;
	double rampDistance, rampFactor;
	double minimumHeight;
};

}

#endif /* CLAWTOANGLE_H_ */
