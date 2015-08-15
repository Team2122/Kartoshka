/**
 * @file ShuttleDown.h
 * @author Alex Mikhalev
 * @date 2/14/15
 */

#ifndef SHUTTLEDOWN_H_
#define SHUTTLEDOWN_H_

#include "Robot.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ShuttleDown: public RobotCommand {
public:
	ShuttleDown(std::string name, YAML::Node config) :
			RobotCommand(name) {
		ticks = config["ticks"].as<int>();
		speed = config["speed"].as<double>();
		Requires(robot->shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttleDown";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		robot->shuttle->SetShuttleSpeed(speed);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return robot->shuttle->GetEncoderTicks() <= ticks
				|| robot->shuttle->GetLimit() == Shuttle::kLower;
	}

	void End() override {
		robot->shuttle->SetShuttleSpeed(Shuttle::Speed::kHold);
		RobotCommand::End();
	}

	void Interrupted() override {
		robot->shuttle->SetShuttleSpeed(Shuttle::Speed::kHold);
		RobotCommand::Interrupted();
	}

private:
	int ticks;
	double speed;
};

}

#endif
