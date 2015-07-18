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
		ticks = config["ticks"].as<int>();
		speed = config["speed"].as<double>();
		Requires(shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttleDown";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		shuttle->SetShuttleSpeed(speed);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return shuttle->GetEncoderTicks() <= ticks
				|| shuttle->GetLimit() == Shuttle::kLower;
	}

	void End() override {
		shuttle->SetShuttleSpeed(Shuttle::Speed::kHold);
		CommandBase::End();
	}

	void Interrupted() override {
		shuttle->SetShuttleSpeed(Shuttle::Speed::kHold);
		CommandBase::Interrupted();
	}

private:
	int ticks;
	double speed;
};

}

#endif
