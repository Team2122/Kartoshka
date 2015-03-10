/**
 * @file ManualClawLift.h
 * @author Nick Hodes
 */

#ifndef MANUALCLAWLIFT_H
#define MANUALCLAWLIFT_H

#include "CommandBase.h"

namespace tator {
class ManualClawLift: public CommandBase {
public:
	ManualClawLift(std::string name, YAML::Node config) :
			CommandBase(name) {
		liftSpeed = (Claw::LiftSpeed) config["speed"].as<int>();
	}

	static std::string GetBaseName() {
		return "ManualClawLift";
	}

protected:
	Claw::LiftSpeed liftSpeed;

	void Initialize() override {
		CommandBase::Initialize();
		claw->SetLiftSpeed(liftSpeed);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		if (liftSpeed == Claw::LiftSpeed::kUp) {
			return claw->IsTop();
		} else if (liftSpeed == Claw::LiftSpeed::kDown) {
			return claw->IsHome();
		} else {
			return true;
		}
	}

	void End() override {
		CommandBase::End();
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
	}
};

} /* namespace tator */

#endif /* MANUALCLAWLIFT_H */
