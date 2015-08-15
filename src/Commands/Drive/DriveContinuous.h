#ifndef DRIVECONTINUOUS_H_
#define DRIVECONTINUOUS_H_

#include "Robot.h"
#include <Joystick.h>
#include "Subsystems/Drive.h"

namespace tator {

class DriveContinuous: public RobotCommand {
public:
	DriveContinuous(std::string name, YAML::Node config) :
			RobotCommand(name) {
		int joystickPort = config["joystick"].as<int>();
		Requires(robot->drive);
		YAML::Node axes = config["axes"];
		axisLeft = axes["left"].as<int>();
		axisRight = axes["right"].as<int>();
		joystick = new Joystick(joystickPort);
	}

	static std::string GetBaseName() {
		return "DriveContinuous";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		robot->drive->SetControlMode(Drive::Mode::direct);
	}

	void Execute() override {
		float speedLeft = joystick->GetRawAxis(axisLeft);
		float speedRight = joystick->GetRawAxis(axisRight);
		robot->drive->SetSpeeds(-speedLeft, -speedRight);
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		robot->drive->SetSpeeds(0, 0);
		robot->drive->SetControlMode(Drive::Mode::direct);
		RobotCommand::End();
	}

	void Interrupted() override {
		robot->drive->SetSpeeds(0, 0);
		robot->drive->SetControlMode(Drive::Mode::direct);
		RobotCommand::Interrupted();
	}

private:
	Joystick* joystick;
	int axisLeft;
	int axisRight;
};

}

#endif
