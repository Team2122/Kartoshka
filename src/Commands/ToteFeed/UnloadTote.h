/**
 * @file RecieveBottomTote.h
 * @date Jan 24, 2015
 * @author Lee Bousfield
 */

#ifndef UNLOADTOTE_H_
#define UNLOADTOTE_H_

#include "Subsystems/Shuttle.h"
#include "Subsystems/ToteFeed.h"
#include "Robot.h"

namespace tator {

class UnloadTote: public RobotCommand {
public:
	UnloadTote(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->toteFeed);
		rollerSpeed = config["rollerSpeed"].as<double>();
		flapperSpeed = config["flapperSpeed"].as<double>();
	}

	static std::string GetBaseName() {
		return "UnloadTote";
	}

protected:
	void Initialize() override {
		RobotCommand::Initialize();
		robot->toteFeed->SetRollers(rollerSpeed);
		robot->toteFeed->SetFlapperSpeed(flapperSpeed);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		RobotCommand::End();
		this->Reset();
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		this->Reset();
	}

	void Reset() {
		robot->shuttle->SetDesiredTotes(6);
		robot->shuttle->SetTotesHeld(robot->shuttle->GetTotesRatcheted());
		robot->toteFeed->SetRollers(0);
		robot->toteFeed->SetFlapperSpeed(0);
	}

private:
	double rollerSpeed, flapperSpeed;
};

}

#endif /* UNLOADTOTE_H_ */
