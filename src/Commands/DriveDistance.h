/**
 * @file DriveDistance.h
 * @author Nick Hodes
 */

#ifndef DRIVEDISTANCE_H_
#define DRIVEDISTANCE_H_

#include "CommandBase.h"
#include "Subsystems/Drive.h"

namespace tator {

class DriveDistance: public CommandBase {
public:
	DriveDistance(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(drive);
		currentDistance = 0;
		startDistance = 0;
		speed = config["speed"].as<double>();
		distance = config["distance"].as<double>();
	}

	static std::string GetBaseName() {
		return "DriveDistance";
	}

protected:
	double currentDistance, startDistance, distance, speed;

	void Initialize() override {
		CommandBase::Initialize();
		startDistance = drive->GetDistance();
	}

	void Execute() override {
		drive->SetSpeeds(speed, speed);
		currentDistance = drive->GetDistance();
	}

	bool IsFinished() override {
		return fabs(currentDistance - startDistance) >= distance;
	}

	void End() override {
		CommandBase::End();
		drive->SetSpeeds(0, 0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		drive->SetSpeeds(0, 0);
	}
};

}

#endif
