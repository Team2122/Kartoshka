/**
 * @file BumpBottomTote.h
 * @date Mar 20, 2015
 * @author Zachary Snow
 */

#ifndef BUMPBOTTOMTOTE_H_
#define BUMPBOTTOMTOTE_H_

#include "Robot.h"
#include "Subsystems/ToteFeed.h"

namespace tator {

class BumpBottomTote: public RobotCommand {
public:
	BumpBottomTote(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->toteFeed);
		time = config["time"].as<double>();
		rollerSpeed = config["rollerSpeed"].as<double>();
		reversed = false;
	}

	static std::string GetBaseName() {
		return "BumpBottomTote";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		reversed = false;
		timer.Reset();
		timer.Start();
	}

	void Execute() override {
		robot->toteFeed->SetRollers(reversed ? -rollerSpeed : rollerSpeed);
	}

	bool IsFinished() override {
		if (timer.HasPeriodPassed(time)) {
			if (reversed) {
				return true;
			}
			reversed = true;
			timer.Reset();
		}
		return false;
	}

	void End() override {
		RobotCommand::End();
		robot->toteFeed->SetRollers(0);
		timer.Stop();
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		robot->toteFeed->SetRollers(0);
		timer.Stop();
	}

private:
	Timer timer;
	double rollerSpeed, time;
	bool reversed;
};

}

#endif /* BUMPBOTTOMTOTE_H_ */
