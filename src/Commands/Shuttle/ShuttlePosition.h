/**
 * @file ShuttlePosition.h
 * @author Alex Mikhalev
 * @Date 2/9/2015
 */

#ifndef SHUTTLEPOSITION_H_
#define SHUTTLEPOSITION_H_

#include "Robot.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ShuttlePosition: public RobotCommand {
public:
	ShuttlePosition(std::string name, YAML::Node config) :
			RobotCommand(name) {
		targetTicks = config["ticks"].as<int>();
		speed = Shuttle::kStop;
		Requires(robot->shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttlePosition";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		int shuttleTicks = robot->shuttle->GetEncoderTicks();
		const char* name;
		if (targetTicks > shuttleTicks) {
			speed = Shuttle::kUp;
			name = "up";
		} else {
			speed = Shuttle::kDown;
			name = "down";
		}
		robot->shuttle->SetShuttleSpeed(speed);
		log.Info("We are moving %s from %d to %d ticks", name, shuttleTicks,
				targetTicks);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		int shuttleTicks = robot->shuttle->GetEncoderTicks();
		int limit = robot->shuttle->GetLimit();
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
	int targetTicks;
	Shuttle::Speed speed;
};

}

#endif
