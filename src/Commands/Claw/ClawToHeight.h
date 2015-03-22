/**
 * @file ClawToHeight.h
 * @date March 13, 2015
 * @author Alex Mikhalev
 */

#ifndef CLAWTOHEIGHT_H_
#define CLAWTOHEIGHT_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {
class ClawToHeight: public CommandBase {
public:
	ClawToHeight(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(claw);

		height = config["height"].as<double>();
		YAML::Node ramp = config["ramp"];
		rampDistance = ramp["distance"].as<double>();
		rampFactor = ramp["factor"].as<double>();
		speed = config["speed"].as<double>();
		requiredAngleName = config["requiredAngle"].as<std::string>();
		requiredAngle = claw->AngleFromName(requiredAngleName);

		direction = 1;
	}

	static std::string GetBaseName() {
		return "ClawToHeight";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		if (!claw->IsAtAngle(requiredAngle)) {
			log.Error("We cannot move the claw lift to this position when we "
					"are not at %s angle", requiredAngleName.c_str());
			return this->Cancel();
		}
		double liftHeight = claw->GetLiftEncoder();
		const char* name;
		if (height >= liftHeight) { // positive is verticle
			name = "up";
		} else {
			name = "down";
		}
		log.Info("We are moving %s to %f ticks", name, height);
	}

	void Execute() override {
		double liftHeight = claw->GetLiftEncoder();
		if (height >= liftHeight) { // calculate the sign of the direction
			direction = -1;
		} else {
			direction = 1;
		}
		if (direction < 0 && claw->IsAtTop()) { // if we're going up
			log.Warn("Upper limit was tripped"); // let them know and die
			return this->Cancel();
		} else if (direction > 0 && claw->IsAtHome()) { // down
			log.Warn("Lower limit was tripped");
			return this->Cancel();
		}

		double difference = fabs(liftHeight - height);
		// Ramp when we are within the ramp distance, but not if that distance is zero
		if (difference <= rampDistance && rampDistance != 0) {
			// this is 1 when we should be at speed, and 0 when we should be at speed * rampFactor
			double ratio = difference / rampDistance;
			// this is derived from a basic lerp function
			double rampedSpeed = (rampFactor + (1 - rampFactor) * ratio) * speed;
			claw->SetLiftSpeed(rampedSpeed * direction);
		} else {
			// if we're not within the ramp distance, just run normally
			claw->SetLiftSpeed(speed * direction);
		}
	}

	bool IsFinished() override {
		return claw->IsAtHeight(height);
	}

	void End() override {
		CommandBase::End();
		claw->SetLiftSpeed(0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		claw->SetLiftSpeed(0);
	}

private:
	double height;
	double direction;
	double rampDistance, rampFactor;
	double speed;
	std::string requiredAngleName;
	Claw::ClawAngle requiredAngle;
};

}

#endif /* CLAWTOHEIGHT_H_ */

