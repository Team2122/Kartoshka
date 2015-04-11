/**
 * @file DriveStraight.h
 * @author Alex Mikhalev
 * @date 3/21/2015
 */

#ifndef DRIVESTRAIGHT_H_
#define DRIVESTRAIGHT_H_

#include "CommandBase.h"
#include "Subsystems/Otto.h"
#include "Subsystems/Drive.h"

namespace tator {

class DriveStraight: public CommandBase {
public:
	DriveStraight(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(drive);
		currentDistance = 0;
		startDistance = 0;
		targetAngle = config["angle"].as<double>();
		speed = config["speed"].as<double>();
		distance = config["distance"].as<double>();
		speedOffset = config["speedOffset"].as<double>();
		angleTolerance = config["angleTolerance"].as<double>();
	}

	static std::string GetBaseName() {
		return "DriveStraight";
	}

protected:
	double targetAngle, currentDistance, startDistance;
	void Initialize() override {
		CommandBase::Initialize();
		startDistance = drive->GetDistance();
	}

	void Execute() override {
		double angle = otto->GetAngle() - targetAngle;
		if (angle >= angleTolerance) {
			drive->SetSpeeds(speed - speedOffset, speed);
		} else if (angle <= -angleTolerance) {
			drive->SetSpeeds(speed, speed - speedOffset);
		} else {
			drive->SetSpeeds(speed, speed);
		}
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

private:
	double distance, speed, speedOffset, angleTolerance;
};

}

#endif /* DRIVESTRAIGHT_H_ */
