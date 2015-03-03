/**
 * @file ClawForceHome.h
 * @author Paul Vaughan
 * @date 2/15/2015
 */

#ifndef CLAWFORCEHOME_H
#define CLAWFORCEHOME_H

#include "CommandBase.h"
#include "Common/Kremlin.h"

namespace tator {

class ClawForceHome: public CommandBase {
public:
	ClawForceHome(std::string name, YAML::Node config) :
			CommandBase(name) {
		speed = config["speed"].as<double>();
		stopAngle = config["angle"].as<double>();
	}

	static std::string GetBaseName() {
		return "ClawForceHome";
	}

protected:
	double stopAngle, speed;
	virtual void Initialize() {
		CommandBase::Initialize();
		if (claw->HasContainer()) {
			log.Warn(
					"You tried to force home the claw while you have a container. "
							"This is bad news. Please take it out");
			this->Cancel();
		}
		Kremlin::Get("ClawRotationContinuous")->Cancel();
		claw->ReenableClaw();
		claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
	}
	virtual void Execute() {
		claw->SetRotationSpeed(speed, true);
	}
	virtual bool IsFinished() {
		return claw->GetRotationAngle() <= stopAngle;
	}
	virtual void End() {
		claw->SetRotationSpeed(0, true);
		Kremlin::Get("ClawRotationContinuous")->Start();
		Kremlin::Get("ClawRotationPick")->Start();
		Kremlin::Get("HomeClaw")->Start();
		CommandBase::End();
	}
	virtual void Interrupted() {
		claw->SetRotationSpeed(0, true);
		Kremlin::Get("ClawRotationContinuous")->Start();
		CommandBase::Interrupted();
	}
};

}

#endif
