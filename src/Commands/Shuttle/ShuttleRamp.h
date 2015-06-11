/**
 * @file ShuttleRamp.h
 * @author Alex Mikhalev
 * @Date 2/9/2015
 */

#ifndef SHUTTLERAMP_H_
#define SHUTTLERAMP_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ShuttleRamp: public CommandBase {
public:
	ShuttleRamp(std::string name, YAML::Node config) :
			CommandBase(name) {
		rampDistance = config["rampDistance"].as<int>();
		targetTicks = config["ticks"].as<int>();
		rampFactor = config["rampFactor"].as<double>();
		speed = Shuttle::kStop;
		Requires(shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttleRamp";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		int shuttleTicks = shuttle->GetEncoderTicks();
		const char* name;
		if (targetTicks > shuttleTicks + tolerance) {
			speed = Shuttle::Speed::kUp;
			name = "up";
		} else if (targetTicks < shuttleTicks - tolerance) {
			speed = Shuttle::Speed::kDown;
			name = "down";
		} else {
			speed = Shuttle::Speed::kStop;
			name = "(just kidding)";
		}
		log.Info("We are moving %s from %d to %d ticks", name, shuttleTicks, targetTicks);
	}

	void Execute() override {
		int shuttleTicks = shuttle->GetEncoderTicks();
		int difference = abs(shuttleTicks - targetTicks);
		if (difference <= rampDistance) {
			double r = (((double) difference) / ((double) rampDistance));
			double s = (rampFactor + (1 - rampFactor) * r)
					* shuttle->GetShuttleSpeed(speed);
			shuttle->SetShuttleSpeed(s);
		} else {
			shuttle->SetShuttleSpeed(speed);
		}
	}

	bool IsFinished() override {
		int shuttleTicks = shuttle->GetEncoderTicks();
		int limit = shuttle->GetLimit();
		switch (speed) {
		case Shuttle::Speed::kUp:
			if (limit == Shuttle::kUpper) {
				log.Warn("Upper limit hit");
				return true;
			}
			return shuttleTicks >= targetTicks;
		case Shuttle::Speed::kDown:
			if (limit == Shuttle::kLower) {
				log.Warn("Lower limit hit");
				return true;
			}
			return shuttleTicks <= targetTicks;
		case Shuttle::Speed::kStop:
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
	const int tolerance = 4;
	double rampFactor;
	int targetTicks, rampDistance;
	Shuttle::Speed speed;
};

}

#endif
