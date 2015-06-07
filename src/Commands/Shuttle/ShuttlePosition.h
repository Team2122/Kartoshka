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
		targetTicks = config["ticks"].as<int>();
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
		if (targetTicks > shuttleTicks) {
			speed = Shuttle::kUp;
			name = "up";
		} else {
			speed = Shuttle::kDown;
			name = "down";
		}
		shuttle->SetShuttleSpeed(speed);
		log.Info("We are moving %s from %d to %d ticks", name, shuttleTicks, targetTicks);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		int shuttleTicks = shuttle->GetEncoderTicks();
		int limit = shuttle->GetLimit();
		switch (speed) {
		case Shuttle::kUp:
			if (limit == Shuttle::kUpper) {
				log.Warn("Upper limit hit");
				return true;
			}
			return shuttleTicks >= targetTicks;
		case Shuttle::kDown:
			if (limit == Shuttle::kLower) {
				log.Warn("Lower limit hit");
				return true;
			}
			return shuttleTicks <= targetTicks;
		default:
			return true;
		}
	}

	void End() override {
		log.Info("Finished while at %d, target of %d", shuttle->GetEncoderTicks(), targetTicks);
		shuttle->SetShuttleSpeed(Shuttle::kHold);
		CommandBase::End();
	}

	void Interrupted() override {
		shuttle->SetShuttleSpeed(Shuttle::kHold);
		CommandBase::Interrupted();
	}

private:
	int targetTicks;
	Shuttle::Speed speed;
};

}

#endif
