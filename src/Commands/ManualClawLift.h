/**
 * @file ManualClawLift.h
 * @author Nick Hodes
 */

#ifndef MANUALCLAWLIFT_H
#define MANUALCLAWLIFT_H

#include "CommandBase.h"

namespace tator {
class ManualClawLift: public CommandBase {
private:
	Claw::LiftSpeed liftSpeed;
public:
	ManualClawLift(std::string name, YAML::Node config) :
			CommandBase(name) {
		liftSpeed = (Claw::LiftSpeed) config["speed"].as<int>();
	}

	void Initialize() {
		CommandBase::Initialize();
		claw->SetLiftSpeed(liftSpeed);
	}
	void Execute() {

	}

	bool IsFinished() {
		if (liftSpeed == Claw::LiftSpeed::kUp) {
			return claw->IsTop();
		} else if (liftSpeed == Claw::LiftSpeed::kDown) {
			return claw->IsHome();
		} else {
			return true;
		}
	}

	void End() {
		CommandBase::End();
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
	}

	void Interrupted() {
		CommandBase::Interrupted();
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
	}

	static std::string GetBaseName() {
		return "ManualClawLift";
	}
};
} /* namespace tator */

#endif /* MANUALCLAWLIFT_H */
