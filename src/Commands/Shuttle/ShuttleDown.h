/**
 * @file ShuttleDown.h
 * @author Alex Mikhalev
 * @date 2/14/15
 */

#ifndef SHUTTLEDOWN_H_
#define SHUTTLEDOWN_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ShuttleDown: public CommandBase {
public:
	ShuttleDown(std::string name, YAML::Node config) :
			CommandBase(name) {
		targetTicks = config["ticks"].as<int>();
		tolerance = config["tolerance"].as<int>();
		speedMul = config["speedMul"].as<double>();
		Requires(shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttleDown";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		shuttle->SetShuttleSpeed(
				shuttle->GetShuttleSpeed(Shuttle::kUp) * speedMul);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return abs(shuttle->GetEncoderTicks() - targetTicks) <= tolerance
				|| shuttle->GetLimit() == Shuttle::kLower;
	}

	void End() override {
		shuttle->SetShuttleSpeed(0);
		CommandBase::End();
	}

	void Interrupted() override {
		shuttle->SetShuttleSpeed(0);
		CommandBase::Interrupted();
	}

private:
	int targetTicks, tolerance;
	double speedMul;
};

}

#endif