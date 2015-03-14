/**
 * @file ClawRamp.h
 * @date March 13, 2015
 * @author Alex Mikhalev
 */

#ifndef CLAWRAMP_H_
#define CLAWRAMP_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {
class ClawRamp: public CommandBase {
public:
	ClawRamp(std::string name, YAML::Node config) :
			CommandBase(name) {
		target = config["target"].as<double>();
		tolerance = config["tolerance"].as<double>();
		rampDistance = config["rampDistance"].as<double>();
		speed = config["speed"].as<double>();
		rampFactor = config["rampFactor"].as<double>();
		direction = 1;
	}

	static std::string GetBaseName() {
		return "ClawRamp";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		double clawPosition = claw->GetLiftEncoder();
		const char* name;
		if (target >= clawPosition) {
			direction = -1;
			name = "up";
		} else {
			direction = 1;
			name = "down";
		}
		log.Info("We are moving %s to %f ticks", name, target);
	}

	void Execute() override {
		double clawPosition = claw->GetLiftEncoder();
		double difference = fabs(clawPosition - target);
		if (difference <= rampDistance) {
			double r = (((double) difference) / ((double) rampDistance));
			double s = (rampFactor + (1 - rampFactor) * r) * speed * direction;
			claw->SetLiftSpeed(s);
		} else {
			claw->SetLiftSpeed(speed * direction);
		}
	}

	bool IsFinished() override {
		double clawPosition = claw->GetLiftEncoder();
		double difference = fabs(clawPosition - target);
		bool limitTripped = true;
		if (direction < 0) {
			limitTripped = claw->IsTop();
		} else {
			limitTripped = claw->IsHome();
		}
		if (limitTripped)
			log.Info("Limit was tripped");
		return difference <= tolerance || limitTripped;
	}

	void End() override {
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
		CommandBase::End();
	}

	void Interrupted() override {
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
		CommandBase::Interrupted();
	}

private:
	double target, tolerance, rampDistance;
	double speed, direction, rampFactor;
}
;

}

#endif /* CLAWRAMP_H_ */

