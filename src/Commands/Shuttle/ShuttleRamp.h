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
		tolerance = config["tolerance"].as<int>();
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
		if (targetTicks >= shuttleTicks) {
			speed = Shuttle::Speed::kUp;
			name = "up";
		} else {
			speed = Shuttle::Speed::kDown;
			name = "down";
		}
		log.Info("We are moving %s to %d ticks", name, targetTicks);
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
		int difference = abs(shuttleTicks - targetTicks);
		int limit = shuttle->GetLimit();
		switch (speed) {
		case Shuttle::Speed::kUp:
			if (limit == Shuttle::kUpper) {
				log.Warn("Upper limit hit");
				return true;
			}
			return difference <= tolerance;
		case Shuttle::Speed::kDown:
			if (limit == Shuttle::kLower) {
				log.Warn("Lower limit hit");
				return true;
			}
			return difference <= tolerance;
		default:
			return true;
		}
	}

	void End() override {
		log.Info("Finished while at %d, target of %d", shuttle->GetEncoderTicks(), targetTicks);
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		CommandBase::End();
	}

	void Interrupted() override {
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		CommandBase::Interrupted();
	}

private:
	double rampFactor;
	int targetTicks, tolerance, rampDistance;
	Shuttle::Speed speed;
};

}

#endif
