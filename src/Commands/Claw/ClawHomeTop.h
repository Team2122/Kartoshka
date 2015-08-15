/*
 * @file ClawHomeTop.h
 * @author Alex Mikhalev
 */

#ifndef CLAWHOMETOP_H_
#define CLAWHOMETOP_H_

#include "Robot.h"

namespace tator {

class ClawHomeTop: public RobotCommand {
public:
	ClawHomeTop(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->claw);

		speed = config["speed"].as<double>();
	}

	static std::string GetBaseName() {
		return "ClawHomeTop";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		if (robot->claw->IsHomed()) {
			log.Error("Claw is already homed!");
			this->Cancel();
		} else {
			log.Info("Homing the claw at the top");
		}
	}

	void Execute() override {
		robot->claw->ForceSetLiftSpeed(speed);
	}

	bool IsFinished() override {
		return robot->claw->IsAtTop();
	}

	void End() override {
		RobotCommand::End();
		robot->claw->SetLiftSpeed(0);
		robot->claw->Home();
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		robot->claw->SetLiftSpeed(0);
	}

private:
	double speed;
};

}

#endif /* CLAWHOMETOP_H_ */
