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
			goingUp = true;
		} else {
			claw->SetLiftSpeed(Claw::LiftSpeed::kDown);
			goingUp = false;
		}
	}

	virtual bool IsFinished() {
		log.Info("Current One: %d Current Two: %d", claw->GetCurrent(4), claw->GetCurrent(10));
		double clawTicks = claw->GetLiftEncoder();
		double difference = abs(clawTicks - target);
		bool limitTripped = true;
		if (goingUp) {
			limitTripped = claw->IsTop();
		} else {
			limitTripped = claw->IsHome();
		}
		if (limitTripped)
			log.Info("Limit was tripped");
		return difference <= tolerance || limitTripped;
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
	bool goingUp;
};
}

#endif /* CLAWPOSITION_H_ */

