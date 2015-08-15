/**
 * @file DriveRotate.h
 * @author Paul Vaughan
 */

#ifndef DRIVEROTATE_H
#define DRIVEROTATE_H

#include "Robot.h"
#include "Subsystems/Drive.h"
#include "Subsystems/Otto.h"
#include <cmath>

namespace tator {

class DriveRotate: public RobotCommand {
public:
	DriveRotate(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->drive);
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
		RobotCommand::Initialize();
		const char* name;
		if (targetAngle >= 0) {
			name = "clockwise";
		} else {
			name = "counterclockwise";
		}
		angle = robot->otto->GetAngle();
		log.Info("Moving %s for from %f to %f degrees", name, angle,
				targetAngle);
	}

	void Execute() override {
		angle = robot->otto->GetAngle();
		double difference = fabs(angle - targetAngle);
		double rampedSpeed;
		if (difference <= slowAngle) {
			rampedSpeed = speed * slowFactor;
		} else {
			rampedSpeed = speed;
		}
		if (targetAngle >= angle) { // Positive is clockwise
			robot->drive->SetSpeeds(rampedSpeed, -rampedSpeed);
		} else {
			robot->drive->SetSpeeds(-rampedSpeed, rampedSpeed);
		}
	}

	bool IsFinished() override {
		double difference = fabs(angle - targetAngle);
		return difference <= tolerance;
	}

	void End() override {
		RobotCommand::End();
		log.Info("Finished rotating. Angle: %f, target: %f", angle,
				targetAngle);
		robot->drive->SetSpeeds(0, 0);
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		robot->drive->SetSpeeds(0, 0);
	}

private:
	double targetAngle, tolerance, speed, angle, slowAngle, slowFactor;

};

}

#endif
