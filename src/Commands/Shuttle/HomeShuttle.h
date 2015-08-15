/**
 * @file HomeShuttle.h
 * @author Alex Mikhalev
 * @date 2/8/2015
 */

#ifndef HOMESHUTTLE_H_
#define HOMESHUTTLE_H_

#include "Robot.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class HomeShuttle: public RobotCommand {
public:
	HomeShuttle(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->shuttle);
		speed = config["speed"].as<double>();
	}

	static std::string GetBaseName() {
		return "HomeShuttle";
	}

protected:
	void Initialize() override {
		robot->shuttle->SetShuttleSpeed(speed);
		RobotCommand::Initialize();
	}

	void Execute() override {
		if (robot->shuttle->GetLimit() == Shuttle::kUpper) {
			log.Error("We have big problems. The shuttle went up when we "
					"thought it should go down");
			this->Cancel();
		}
	}

	bool IsFinished() override {
		return robot->shuttle->GetLimit() == Shuttle::kLower;
	}

	void End() override {
		robot->shuttle->ResetEncoder();
		robot->shuttle->SetShuttleSpeed(Shuttle::kHold);
		RobotCommand::End();
	}

	void Interrupted() override {
		robot->shuttle->SetShuttleSpeed(Shuttle::kHold);
		RobotCommand::Interrupted();
	}

private:
	double speed;
};

}

#endif
