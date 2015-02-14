/**
 * @file DriveDetectPlatform.h
 * @author Paul Vaughan
 * @date 2/14/2015
 */

#ifndef DRIVEDETECTPLATFORM_H
#define DRIVEDETECTPLATFORM_H

#include "CommandBase.h"
#include "Subsystems/Drive.h"

namespace tator {

class DriveDetectPlatform: public CommandBase {
public:
	DriveDetectPlatform(std::string name, YAML::Node config) :
			CommandBase(name) {
		finished = false;
		bothSpeed = config["bothSpeed"].as<float>();;
		sideSpeed = config["sideSpeed"].as<float>();;
	}

	virtual void Initialize() {
		CommandBase::Initialize();
		Kremlin::Get("DriveContinuous")->Start();
	}

	virtual void Execute() {
		switch (drive->GetPlatformState()){
		case Drive::PlatformState::none:
			drive->SetSpeeds(bothSpeed, bothSpeed);
			finished = false;
			break;
		case Drive::PlatformState::right:
			drive->SetSpeeds(sideSpeed, 0);
			finished = false;
			break;
		case Drive::PlatformState::left:
			drive->SetSpeeds(0, sideSpeed);
			finished = false;
			break;
		case Drive::PlatformState::both:
			drive->SetSpeeds(0, 0);
			finished = true;
			break;
		default:
			drive->SetSpeeds(0, 0);
			break;
		}
	}

	virtual bool IsFinished() {
		return finished;
	}

	virtual void End() {
		CommandBase::End();
		drive->SetSpeeds(0, 0);
		Kremlin::Get("DriveContinuous")->Start();
	}

	virtual void Interrupted() {
		CommandBase::Interrupted();
		drive->SetSpeeds(0, 0);
		Kremlin::Get("DriveContinuous")->Start();
	}

	static std::string GetBaseName() {
		return "DriveDetectPlatform";
	}
protected:
	bool finished;
	float bothSpeed, sideSpeed;
};

}

#endif
