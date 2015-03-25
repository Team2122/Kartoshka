/**
 * @file ManualClawLift.h
 * @author Nick Hodes
 */

#ifndef MANUALCLAWLIFT_H
#define MANUALCLAWLIFT_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {
class ManualClawLift: public CommandBase {
public:
	ManualClawLift(std::string name, YAML::Node config) :
			CommandBase(name) {
		speed = config["speed"].as<double>();
		Requires(claw);
	}

	static std::string GetBaseName() {
		return "ManualClawLift";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		claw->SetLiftSpeed(speed);
	}

	void Execute() override {
		if (speed < 0 && claw->IsAtTop()) {
			log.Warn("Upper limit was tripped");
			return this->Cancel();
		} else if (speed > 0 && claw->IsAtHome()) {
			log.Warn("Lower limit was tripped");
			return this->Cancel();
		}
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		CommandBase::End();
		claw->SetLiftSpeed(0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		claw->SetLiftSpeed(0);
	}

private:
	double speed;
};

} /* namespace tator */

#endif /* MANUALCLAWLIFT_H */
