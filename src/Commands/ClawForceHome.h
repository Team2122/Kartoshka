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
		stopAngle = config["angle"].as<double>();
	}

	static std::string GetBaseName() {
		return "ClawForceHome";
	}

protected:
	double stopAngle;
	virtual void Initialize() {
		CommandBase::Initialize();
		Kremlin::Get("ClawRotationContinuous")->Cancel();
		claw->ReenableClaw();
	}
	virtual void Execute() {
		claw->SetRotationSpeed(Claw::RotationSpeed::kForward, true);
	}
	virtual bool IsFinished() {
		return claw->GetRotationAngle() <= stopAngle;
	}
	virtual void End() {
		claw->SetRotationSpeed(Claw::RotationSpeed::kStopped, true);
		Kremlin::Get("ClawRotationContinuous")->Start();
		Kremlin::Get("HomeClaw")->Start();
		CommandBase::End();
	}
	virtual void Interrupted() {
		claw->SetRotationSpeed(Claw::RotationSpeed::kStopped, true);
		Kremlin::Get("ClawRotationContinuous")->Start();
		CommandBase::Interrupted();
	}
};

}

#endif
