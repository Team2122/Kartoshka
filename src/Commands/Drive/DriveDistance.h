/**
 * @file DriveDistance.h
 * @author Nick Hodes
 */

#ifndef DRIVEDISTANCE_H_
#define DRIVEDISTANCE_H_

#include "Robot.h"
#include "Subsystems/Drive.h"

namespace tator {

class DriveDistance: public RobotCommand {
public:
	DriveDistance(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->drive);
		currentDistance = 0;
		startDistance = 0;
		speed = config["speed"].as<double>();
		distance = config["distance"].as<double>();
	}

	static std::string GetBaseName() {
		return "DriveDistance";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		startDistance = robot->drive->GetDistance();
	}

	void Execute() override {
		robot->drive->SetSpeeds(speed, speed);
		currentDistance = robot->drive->GetDistance();
	}

	bool IsFinished() override {
		return fabs(currentDistance - startDistance) >= distance;
	}

	void End() override {
		RobotCommand::End();
		robot->drive->SetSpeeds(0, 0);
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		robot->drive->SetSpeeds(0, 0);
	}

private:
	double currentDistance, startDistance, distance, speed;
};

}

#endif
