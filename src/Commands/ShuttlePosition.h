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
	ShuttlePosition(YAML::Node config) :
			CommandBase(GetBaseName()) {
		targetTicks = config["ticks"].as<int32_t>();
	}

	static std::string GetBaseName() {
		return "ShuttlePosition";
	}

	virtual void Initialize() {
		// CommandBase::Initialize();
		auto shuttleTicks = shuttle->GetEncoderTicks();
		if (targetTicks >= shuttleTicks) {
			shuttle->SetShuttleSpeed(Shuttle::kUp);
			direction = kUp;
		} else {
			shuttle->SetShuttleSpeed(Shuttle::kDown);
			direction = kDown;
		}
	}

	virtual void Execute() {
		if (shuttle->IsStalled()) {
			log.Error("The shuttle has stalled while traveling to a position");
			Cancel();
			return;
		}
		auto limit = shuttle->GetLimit();
		switch (limit) {
		case Shuttle::kUpper:
		case Shuttle::kLower: {
			auto name = (limit == Shuttle::kUpper) ? "upper" : "lower";
			log.Error("The shuttle has hit the %s limit while traveling to a "
					"position", name);
			Cancel();
			break;
		}
		default:
			break;
		}
	}

	virtual bool IsFinished() {
		auto shuttleTicks = shuttle->GetEncoderTicks();
		switch (direction) {
		case kUp:
			return shuttleTicks >= targetPosition;
		case kDown:
			return shuttleTicks <= targetPosition;
		default:
			return true;
		}
	}

	virtual void End() {
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		// CommandBase::End();
	}

	virtual void Interrupted() {
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		// CommandBase::Interrupted();
	}

protected:
	int32_t targetTicks;
	enum Direction {
		kUp, kDown
	};
	Direction direction;
};

}

#endif
