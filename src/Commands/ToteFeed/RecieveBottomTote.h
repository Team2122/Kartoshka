/**
 * @file RecieveBottomTote.h
 * @date Jan 24, 2015
 * @author Lee Bousfield
 */

#ifndef RECIEVETOTE_H_
#define RECIEVETOTE_H_

#include "Subsystems/ToteFeed.h"
#include "Robot.h"

namespace tator {

class RecieveBottomTote: public RobotCommand {
public:
	RecieveBottomTote(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->toteFeed);
		flapperSpeed = config["flapperSpeed"].as<double>();
		rollerSpeed = config["rollerSpeed"].as<double>();
	}

	static std::string GetBaseName() {
		return "RecieveBottomTote";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		robot->toteFeed->SetFlapperSpeed(flapperSpeed);
		robot->toteFeed->SetRollers(rollerSpeed);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		RobotCommand::End();
		robot->toteFeed->SetFlapperSpeed(0);
		robot->toteFeed->SetRollers(0);
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		robot->toteFeed->SetFlapperSpeed(0);
		robot->toteFeed->SetRollers(0);
	}

private:
	double flapperSpeed;
	double rollerSpeed;
};

}

#endif /* RECIEVETOTE_H_ */
