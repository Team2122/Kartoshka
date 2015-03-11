/**
 * @file ClawPosition.h
 * @date Feb 15, 2015
 * @author Lee Bousfield
 */

#ifndef CLAWPOSITION_H_
#define CLAWPOSITION_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {
class ClawPosition: public CommandBase {
public:
	ClawPosition(std::string name, YAML::Node node) :
			CommandBase(name) {
		target = node["target"].as<double>();
		tolerance = node["tolerance"].as<double>();
		speedMul = node["speedMul"].as<double>();
	}

	static std::string GetBaseName() {
		return "ClawPosition";
	}

protected:
	void Execute() override {
		int clawTicks = claw->GetLiftEncoder();
		double speed;
		if (clawTicks < target) {
			speed = claw->GetLiftSpeed(Claw::LiftSpeed::kUp);
		} else {
			speed = claw->GetLiftSpeed(Claw::LiftSpeed::kDown);
		}
		claw->SetLiftSpeed(speed * speedMul);
	}

	bool IsFinished() override {
		double clawTicks = claw->GetLiftEncoder();
		double difference = abs(clawTicks - target);
		bool limitTripped = true;
		switch (claw->GetLiftSpeed()) {
		case Claw::LiftSpeed::kUp:
			limitTripped = claw->IsTop();
			break;
		case Claw::LiftSpeed::kDown:
			limitTripped = claw->IsHome();
			break;
		default:
			limitTripped = false;
			break;
		}
		if (limitTripped)
			log.Info("Limit was tripped");
		return difference <= tolerance || limitTripped;
	}

	void End() override {
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
		CommandBase::End();
		log.Info("Current ticks: %f", claw->GetLiftEncoder());
	}

	void Interrupted() override {
		claw->SetLiftSpeed(Claw::LiftSpeed::kStop);
		CommandBase::Interrupted();
		log.Info("Current ticks: %f", claw->GetLiftEncoder());
	}

private:
	double target;
	double tolerance;
	double speedMul;
};

}

#endif /* CLAWPOSITION_H_ */

