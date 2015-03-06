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
private:
	double currentDistance;
	double startDistance;
	double speed;
	double distance;
	Timer time;
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

	virtual void Initialize() {
		CommandBase::Initialize();
		time.Reset();
		time.Start();
		startDistance = drive->GetDistance();
	}

	virtual void Execute() {
		drive->SetSpeeds(speed, speed);
		currentDistance = drive->GetDistance();
	}

	virtual bool IsFinished() {
		return fabs(currentDistance - startDistance) >= distance;
	}

	virtual void End() {
		CommandBase::End();
		drive->SetSpeeds(0, 0);
	}

	virtual void Interrupted() {
		CommandBase::Interrupted();
		drive->SetSpeeds(0, 0);
	}

protected:
};

}

#endif
