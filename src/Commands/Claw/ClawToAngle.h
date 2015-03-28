/**
 * @file ClawToAngle.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef CLAWTOANGLE_H_
#define CLAWTOANGLE_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawToAngle: public CommandBase {
public:
	ClawToAngle(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(claw);

		angle = claw->AngleFromName(config["angle"].as<std::string>());
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
		CommandBase::Initialize();
		// If we're above the platform
		// and not already at the angle
		if (claw->GetLiftEncoder() < minimumHeight && !claw->IsAtAngle(angle)) {
			log.Error("We cannot rotate if the height of the claw is less "
					"than %f", minimumHeight);
			return this->Cancel();
		}
	}

	void Execute() override {
		double distance = claw->GetDegreesFromAngle(angle);
		double multiplier = 1;
		if (distance <= rampDistance) {
			multiplier = rampFactor;
		}
		claw->SetRotationSpeed(speed * multiplier);
	}

	bool IsFinished() override {
		return claw->IsAtAngle(angle);
	}

	void End() override {
		CommandBase::End();
		claw->SetRotationSpeed(holdSpeed);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		claw->SetRotationSpeed(0);
	}

private:
	Claw::ClawAngle angle;
	double speed, holdSpeed;
	double rampDistance, rampFactor;
	double minimumHeight;
};

}

#endif /* CLAWTOANGLE_H_ */
