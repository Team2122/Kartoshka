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
		targetAngle = config["angle"].as<double>();
		tolerance = config["tolerance"].as<double>();
		slowAngle = config["slowAngle"].as<double>();
		slowFactor = config["slowFactor"].as<double>();
		angle = 0;
	}

	static std::string GetBaseName() {
		return "DriveRotate";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		const char* name;
		if (targetAngle >= 0) {
			name = "clockwise";
		} else {
			name = "counterclockwise";
		}
		log.Info("Moving %s for %f degrees", name, targetAngle);
	}

	void Execute() override {
		angle = otto->GetAngle();
		double difference = fabs(angle - targetAngle);
		double rampedSpeed;
		if (difference <= slowAngle) {
			rampedSpeed = speed * slowFactor;
		} else {
			rampedSpeed = speed;
		}
		if (targetAngle >= angle) { // Positive is clockwise
			drive->SetSpeeds(rampedSpeed, -rampedSpeed);
		} else {
			drive->SetSpeeds(-rampedSpeed, rampedSpeed);
		}
	}

	bool IsFinished() override {
		double difference = fabs(angle - targetAngle);
		return difference <= tolerance;
	}

	void End() override {
		CommandBase::End();
		log.Info("Finished rotating. target: %f, actual: %f", targetAngle,
				angle);
		drive->SetSpeeds(0, 0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		drive->SetSpeeds(0, 0);
	}

private:
	double targetAngle, tolerance, speed, angle, slowAngle,
			slowFactor;

};

}

#endif
