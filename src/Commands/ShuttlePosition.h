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
		CommandBase::Initialize();
		auto shuttleTicks = shuttle->GetEncoderTicks();
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
		auto limit = shuttle->GetLimit();
		switch (limit) {
		case Shuttle::kUpper:
			log.Error("The shuttle has hit the upper limit while traveling to"
					" a position");
			Cancel();
			break;
		default:
			break;
		}
	}

	virtual bool IsFinished() {
		auto shuttleTicks = shuttle->GetEncoderTicks();
		switch (direction) {
		case kUp:
			return shuttleTicks >= targetTicks;
		case kDown:
			return shuttleTicks <= targetTicks;
		default:
			return true;
		}
	}

	virtual void End() {
		shuttle->SetShuttleSpeed(Shuttle::kHold);
		CommandBase::End();
	}

	virtual void Interrupted() {
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		CommandBase::Interrupted();
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
