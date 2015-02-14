/**
 * @file ClawPosition.h
 * @date Feb 15, 2015
 * @author Lee Bousfield
 */

#include "CommandBase.h"

#ifndef CLAWPOSITION_H_
#define CLAWPOSITION_H_

namespace tator {
class ClawPosition: public CommandBase {
public:
	ClawPosition(std::string name, YAML::Node node) :
			CommandBase(name) {
		target = node["target"].as<double>();
		tolerance = node["tolerance"].as<double>();
	}

	static std::string GetBaseName() {
		return "ClawPosition";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
	}

	virtual void Execute() {
		int clawTicks = claw->GetLiftEncoder();
		if (clawTicks < target) {
			claw->SetLiftSpeed(Claw::LiftSpeed::kUp);
		} else {
			claw->SetLiftSpeed(Claw::LiftSpeed::kDown);
		}
	}

	virtual bool IsFinished() {
		double clawTicks = claw->GetLiftEncoder();
		double difference = abs(clawTicks - target);
		return difference <= tolerance;
	}

	virtual void End() {
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
		CommandBase::End();
	}

	virtual void Interrupted() {
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
		CommandBase::Interrupted();
	}
private:
	double target;
	double tolerance;
};
}

#endif /* CLAWPOSITION_H_ */

