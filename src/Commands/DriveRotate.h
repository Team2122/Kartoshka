/**
 * @file DriveRotate.h
 * @author Paul Vaughan
 */

#ifndef DRIVEROTATE_H
#define DRIVEROTATE_H

#include "CommandBase.h"
#include "Subsystems/Drive.h"
#include "Subsystems/Otto.h"
#include <cmath>

namespace tator {

class DriveRotate: public CommandBase {
public:
	DriveRotate(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(drive);
		speed = config["speed"].as<double>();
		deltaAngle = config["angle"].as<double>();
		tolerance = config["tolerance"].as<double>();
		slowAngle = config["slowAngle"].as<double>();
		slowFactor = config["slowFactor"].as<double>();
		startAngle = 0, angle = 0;
	}

	static std::string GetBaseName() {
		return "DriveRotate";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		// We store this because we want to operate on an angle delta instead of an absolute angle
		startAngle = otto->GetAngle();
		const char* name;
		if (deltaAngle >= 0) {
			name = "clockwise";
		} else {
			name = "counterclockwise";
		}
		log.Info("Moving %s for %f degrees", name, deltaAngle);
	}

	void Execute() override {
		angle = otto->GetAngle() - startAngle;
		double difference = fabs(angle - deltaAngle);
		double rampedSpeed;
		if (difference <= slowAngle) {
			rampedSpeed = speed * slowFactor;
		} else {
			rampedSpeed = speed;
		}
		if (deltaAngle >= angle) { // Positive is clockwise
			drive->SetSpeeds(rampedSpeed, -rampedSpeed);
		} else {
			drive->SetSpeeds(-rampedSpeed, rampedSpeed);
		}
	}

	bool IsFinished() override {
		double difference = fabs(angle - deltaAngle);
		return difference <= tolerance;
	}

	void End() override {
		CommandBase::End();
		log.Info("Finished rotating. target: %f, actual: %f", deltaAngle,
				angle);
		drive->SetSpeeds(0, 0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		drive->SetSpeeds(0, 0);
	}

private:
	double startAngle, deltaAngle, tolerance, speed, angle, slowAngle,
			slowFactor;

};

}

#endif
