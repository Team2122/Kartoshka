/**
 * @file ClawForceHome.h
 * @author Paul Vaughan
 * @date 2/15/2015
 */

#ifndef CLAWFORCEHOME_H
#define CLAWFORCEHOME_H

#include "CommandBase.h"
#include "Common/Kremlin.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawForceHome: public CommandBase {
public:
	ClawForceHome(std::string name, YAML::Node config) :
			CommandBase(name) {
		speed = config["speed"].as<double>();
		stopAngle = config["angle"].as<double>();
		clawRotationContinuousCommand = Kremlin::Get("ClawRotationContinuous");
		clawRotationPickCommand = Kremlin::Get("ClawRotationPick");
		homeClawCommand = Kremlin::Get("HomeClaw");
	}

	static std::string GetBaseName() {
		return "ClawForceHome";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		if (claw->HasContainer()) {
			log.Warn(
					"You tried to force home the claw while you have a container. "
							"This is bad news. Please take it out");
			this->Cancel();
		}
		clawRotationContinuousCommand->Cancel(); // we rotate manually, so disable this
		claw->ReenableClaw();
		claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
	}

	void Execute() override {
		claw->SetRotationSpeed(speed, true);
	}

	bool IsFinished() override {
		return claw->GetRotationAngle() <= stopAngle;
	}

	void End() override {
		claw->SetRotationSpeed(0, true);
		clawRotationContinuousCommand->Start();
		clawRotationPickCommand->Start();
		homeClawCommand->Start();
		CommandBase::End();
	}

	void Interrupted() override {
		claw->SetRotationSpeed(0, true);
		clawRotationContinuousCommand->Start();
		CommandBase::Interrupted();
	}

private:
	Command* clawRotationContinuousCommand;
	Command* clawRotationPickCommand;
	Command* homeClawCommand;
	double stopAngle, speed;
};

}

#endif
