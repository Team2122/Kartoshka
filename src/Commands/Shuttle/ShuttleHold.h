/**
 * @file ShuttleHold.h
 * @author Alex Mikhalev
 * @date March 21, 2015
 */

#ifndef SHUTTLEHOLD_H_
#define SHUTTLEHOLD_H_

#include "Robot.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ShuttleHold: public RobotCommand {
public:
	ShuttleHold(std::string name, YAML::Node config) :
			RobotCommand(name) {
		speed = config["speed"].as<double>();
		time = config["time"].as<double>();
		Requires(robot->shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttleHold";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		timer.Reset();
		timer.Start();
	}

	void Execute() override {
		robot->shuttle->SetShuttleSpeed(speed);
	}

	bool IsFinished() override {
		return timer.HasPeriodPassed(time);
	}

	void End() override {
		RobotCommand::End();
		timer.Stop();
		robot->shuttle->SetShuttleSpeed(0);
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		timer.Stop();
		robot->shuttle->SetShuttleSpeed(0);
	}

private:
	Timer timer;
	double speed, time;
};

}

#endif /* SHUTTLEHOLD_H_ */
