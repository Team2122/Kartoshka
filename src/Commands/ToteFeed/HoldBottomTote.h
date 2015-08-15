/**
 * @file HoldBottomTote.h
 * @date Mar 20, 2015
 * @author Zachary Snow
 */

#ifndef HOLDBOTTOMTOTE_H_
#define HOLDBOTTOMTOTE_H_

#include "Subsystems/ToteFeed.h"
#include "Robot.h"

namespace tator {

class HoldBottomTote: public RobotCommand {
public:
	HoldBottomTote(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->toteFeed);
		rollerSpeedSlow = config["rollerSpeed"]["slow"].as<double>();
		rollerSpeedFast = config["rollerSpeed"]["fast"].as<double>();
	}

	static std::string GetBaseName() {
		return "HoldBottomTote";
	}

protected:
	void Execute() override {
		if (robot->toteFeed->GetBackSensor()) {
			robot->toteFeed->SetRollers(rollerSpeedSlow);
		} else {
			robot->toteFeed->SetRollers(rollerSpeedFast);
		}
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		RobotCommand::End();
		robot->toteFeed->SetRollers(0);
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		robot->toteFeed->SetRollers(0);
	}

private:
	double rollerSpeedSlow;
	double rollerSpeedFast;
};

}

#endif /* HOLDBOTTOMTOTE_H_ */
