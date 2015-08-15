/**
 * @file ManualClawLift.h
 * @author Nick Hodes
 */

#ifndef MANUALCLAWLIFT_H
#define MANUALCLAWLIFT_H

#include "Robot.h"
#include "Subsystems/Claw.h"

namespace tator {
class ManualClawLift: public RobotCommand {
public:
	ManualClawLift(std::string name, YAML::Node config) :
			RobotCommand(name) {
		speed = config["speed"].as<double>();
		Requires(robot->claw);
	}

	static std::string GetBaseName() {
		return "ManualClawLift";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		robot->claw->SetLiftSpeed(speed);
	}

	void Execute() override {
		if (speed < 0 && robot->claw->IsAtTop()) {
			log.Warn("Upper limit was tripped");
			return this->Cancel();
		} else if (speed > 0 && robot->claw->IsAtHome()) {
			log.Warn("Lower limit was tripped");
			return this->Cancel();
		}
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		RobotCommand::End();
		robot->claw->SetLiftSpeed(0);
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		robot->claw->SetLiftSpeed(0);
	}

private:
	double speed;
};

} /* namespace tator */

#endif /* MANUALCLAWLIFT_H */
