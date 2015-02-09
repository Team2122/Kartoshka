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
		targetPosition = config["targetPosition"].as<int32_t>();
		tolerance = config["tolerance"].as<int32_t>();
	}

	static std::string GetBaseName() {
		return "ShuttlePosition";
	}

	virtual void Initialize() {
		// CommandBase::Initialize();
		auto shuttlePosition = shuttle->GetEncoderTicks();
		if (targetPosition >= shuttlePosition) {
			shuttle->SetShuttleSpeed(Shuttle::kUp);
		} else {
			shuttle->SetShuttleSpeed(Shuttle::kDown);
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
		auto difference = abs(shuttle->GetEncoderTicks() - targetPosition);
		return difference < tolerance;
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
	int32_t targetPosition, tolerance;
};

}

#endif
