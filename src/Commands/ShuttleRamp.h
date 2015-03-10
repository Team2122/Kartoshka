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
		rampDistance = config["rampDistance"].as<int32_t>();
		targetTicks = config["ticks"].as<int32_t>();
		tolerance = config["tolerance"].as<int32_t>();
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
		if (limit != Shuttle::kUnknown)
			log.Info("Limit hit");
		switch (speed) {
		case Shuttle::Speed::kUp:
			return difference <= tolerance || limit == Shuttle::kUpper;
		case Shuttle::Speed::kDown:
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
	double rampFactor;
	int32_t targetTicks, tolerance, rampDistance;
	Shuttle::Speed speed;
};

}

#endif
