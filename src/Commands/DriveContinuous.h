#include "CommandBase.h"
#include "Subsystems/Drive.h"
#include <WPILib.h>
#include <yaml-cpp/yaml.h>

namespace tator {

class DriveContinuous: public CommandBase {
public:
	DriveContinuous(YAML::Node config) :
			CommandBase(GetBaseName()) {
		int joystickPort = config["joystick"].as<int>();
		axisLeft = config["axes"]["left"].as<int>();
		axisRight = config["axes"]["right"].as<int>();
		joystick = new Joystick(joystickPort);
	}

	void Initialize() {
	}

	void Execute() {
		float speedLeft = joystick->GetRawAxis(axisLeft);
		float speedRight = joystick->GetRawAxis(axisRight);
		drive->SetSpeeds(-speedLeft, -speedRight);
	}

	bool IsFinished() {
		return false;
	}

	void End() {
		drive->SetSpeeds(0, 0);
	}

	void Interrupted() {
		drive->SetSpeeds(0, 0);
	}

	static std::string GetBaseName() {
		return "DriveContinuous";
	}
protected:
	Joystick* joystick;
	int axisLeft;
	int axisRight;
};

}
