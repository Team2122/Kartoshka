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
		speed = Shuttle::kStop;
		Requires(shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttlePosition";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		int shuttleTicks = shuttle->GetEncoderTicks();
		const char* name;
		if (targetTicks >= shuttleTicks) {
			speed = Shuttle::kUp;
			name = "up";
		} else {
			speed = Shuttle::kDown;
			name = "down";
		}
		shuttle->SetShuttleSpeed(speed);
		log.Info("We are moving %s to %d ticks", name, targetTicks);
	}

	void Execute() override {
		if (shuttle->IsStalled()) {
			log.Error("The shuttle has stalled while traveling to a position");
			Cancel();
			return;
		}
	}

	bool IsFinished() override {
		int shuttleTicks = shuttle->GetEncoderTicks();
		int difference = abs(shuttleTicks - targetTicks);
		int limit = shuttle->GetLimit();
		if (limit != Shuttle::kUnknown)
			log.Info("Limit hit");
		switch (speed) {
		case Shuttle::kUp:
			return difference <= tolerance || limit == Shuttle::kUpper;
		case Shuttle::kDown:
			return difference <= tolerance || limit == Shuttle::kLower;
		default:
			return true;
		}
	}

	void End() override {
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		CommandBase::End();
	}

	void Interrupted() override {
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		CommandBase::Interrupted();
	}

private:
	int32_t targetTicks, tolerance;
	Shuttle::Speed speed;
};

}

#endif
