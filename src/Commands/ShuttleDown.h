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

class ShuttleDown : public CommandBase {
public:
	ShuttleDown(std::string name, YAML::Node config) :
		CommandBase(name) {
		targetTicks = config["ticks"].as<int>();
		tolerance = config["tolerance"].as<int>();
		speed = config["speed"].as<double>();
		Requires(shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttleDown";
	}
protected:
	int targetTicks, tolerance;
	double speed;

	virtual void Initialize() {
		CommandBase::Initialize();
		shuttle->SetShuttleSpeed(speed);
	}

	virtual void Execute() {
	}

	virtual bool IsFinished() {
		return abs(shuttle->GetEncoderTicks() - targetTicks) <= tolerance
				|| shuttle->GetLimit() != Shuttle::kUnknown;
	}

	virtual void End() {
		shuttle->SetShuttleSpeed(0);
		CommandBase::End();
	}

	virtual void Interrupted() {
		shuttle->SetShuttleSpeed(0);
		CommandBase::Interrupted();
	}
};

}

#endif
