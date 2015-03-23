#ifndef DRIVECONTINUOUS_H_
#define DRIVECONTINUOUS_H_

#include "CommandBase.h"
#include <Joystick.h>
#include "Subsystems/Drive.h"

namespace tator {

class DriveContinuous: public CommandBase {
public:
	DriveContinuous(std::string name, YAML::Node config) :
			CommandBase(name) {
		int joystickPort = config["joystick"].as<int>();
		Requires(drive);
		YAML::Node axes = config["axes"];
		axisLeft = axes["left"].as<int>();
		axisRight = axes["right"].as<int>();
		joystick = new Joystick(joystickPort);
	}

	static std::string GetBaseName() {
		return "DriveContinuous";
	}

protected:
	void Execute() override {
		float speedLeft = joystick->GetRawAxis(axisLeft);
		float speedRight = joystick->GetRawAxis(axisRight);
		drive->SetSpeeds(-speedLeft, -speedRight);
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		drive->SetSpeeds(0, 0);
		CommandBase::End();
	}

	void Interrupted() override {
		drive->SetSpeeds(0, 0);
		CommandBase::Interrupted();
	}

private:
	Joystick* joystick;
	int axisLeft;
	int axisRight;
};

}

#endif
