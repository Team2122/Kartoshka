/**
 * @file ShuttlePosition.h
 * @author Alex Mikhalev
 * @Date 2/9/2015
 */

#ifndef SHUTTLEPOSITION_H_
#define SHUTTLEPOSITION_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ShuttlePosition: public CommandBase {
public:
	ShuttlePosition(std::string name, YAML::Node config) :
			CommandBase(name) {
		targetTicks = config["ticks"].as<int32_t>();
		tolerance = config["tolerance"].as<int32_t>();
	}

	static std::string GetBaseName() {
		return "ShuttlePosition";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
		int shuttleTicks = shuttle->GetEncoderTicks();
		const char* name;
		if (targetTicks >= shuttleTicks) {
			shuttle->SetShuttleSpeed(Shuttle::kUp);
			direction = kUp;
			name = "up";
		} else {
			shuttle->SetShuttleSpeed(Shuttle::kDown);
			direction = kDown;
			name = "down";
		}
		log.Info("We are moving %s to %d ticks", name, targetTicks);
	}

	virtual void Execute() {
		if (shuttle->IsStalled()) {
			log.Error("The shuttle has stalled while traveling to a position");
			Cancel();
			return;
		}
	}

	virtual bool IsFinished() {
		int shuttleTicks = shuttle->GetEncoderTicks();
		int difference = abs(shuttleTicks - targetTicks);
		int limit = shuttle->GetLimit();
		switch (direction) {
		case kUp:
			return difference <= tolerance || limit == Shuttle::kUpper;
		case kDown:
			return difference <= tolerance || limit == Shuttle::kLower;
		default:
			return true;
		}
	}

	virtual void End() {
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		CommandBase::End();
	}

	virtual void Interrupted() {
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		CommandBase::Interrupted();
	}

protected:
	int32_t targetTicks, tolerance;
	enum Direction {
		kUp, kDown
	};
	Direction direction;
};

}

#endif
