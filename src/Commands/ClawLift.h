/**
 * @file ClawLift.h
 * @date Feb 13, 2015
 * @author Lee Bousfield
 */

#ifndef CLAWLIFT_H_
#define CLAWLIFT_H_

#include "CommandBase.h"
#include <cmath> // For abs()

namespace tator {
class ClawLift : public CommandBase {
public:
	ClawLift(std::string name, YAML::Node config) :
			CommandBase(name) {
		speed = config["speed"].as<double>();
		Requires(claw);
	}

	void Initialize() {
		CommandBase::Initialize();
		claw->SetVerticalLiftMotor(speed);
	}

	void Execute() {
	}

	void End() {
		CommandBase::End();
		claw->SetVerticalLiftMotor(0);
	}

	void Interrupted() {
		CommandBase::Interrupted();
		claw->SetVerticalLiftMotor(0);
	}
protected:
	double speed;
};
}

#endif /* CLAWLIFT_H_ */
