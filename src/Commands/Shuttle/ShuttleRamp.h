/**
 * @file ShuttleRamp.h
 * @author Alex Mikhalev
 * @Date 2/9/2015
 */

#ifndef SHUTTLERAMP_H_
#define SHUTTLERAMP_H_

#include "Robot.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ShuttleRamp: public RobotCommand {
public:
	ShuttleRamp(std::string name, YAML::Node config) :
			RobotCommand(name) {
		rampDistance = config["rampDistance"].as<int>();
		targetTicks = config["ticks"].as<int>();
		rampFactor = config["rampFactor"].as<double>();
		speed = Shuttle::kStop;
		Requires(robot->shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttleRamp";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		int shuttleTicks = robot->shuttle->GetEncoderTicks();
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
		log.Info("We are moving %s from %d to %d ticks", name, shuttleTicks,
				targetTicks);
	}

	void Execute() override {
		int shuttleTicks = robot->shuttle->GetEncoderTicks();
		int difference = abs(shuttleTicks - targetTicks);
		if (difference <= rampDistance) {
			double r = (((double) difference) / ((double) rampDistance));
			double s = (rampFactor + (1 - rampFactor) * r)
					* robot->shuttle->GetShuttleSpeed(speed);
			robot->shuttle->SetShuttleSpeed(s);
		} else {
			robot->shuttle->SetShuttleSpeed(speed);
		}
	}

	bool IsFinished() override {
		int shuttleTicks = robot->shuttle->GetEncoderTicks();
		int limit = robot->shuttle->GetLimit();
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
		log.Info("Finished while at %d, target of %d",
				robot->shuttle->GetEncoderTicks(), targetTicks);
		robot->shuttle->SetShuttleSpeed(Shuttle::kHold);
		RobotCommand::End();
	}

	void Interrupted() override {
		robot->shuttle->SetShuttleSpeed(Shuttle::kHold);
		RobotCommand::Interrupted();
	}

private:
	const int tolerance = 4;
	double rampFactor;
	int targetTicks, rampDistance;
	Shuttle::Speed speed;
};

}

#endif
