/**
 * @file IntakeTotes.h
 * @date Mar 20, 2015
 * @author Zachary Snow
 */

#ifndef INTAKETOTES_H_
#define INTAKETOTES_H_

#include "Subsystems/ToteFeed.h"
#include "Robot.h"

namespace tator {

class IntakeTotes: public RobotCommand {
public:
	IntakeTotes(std::string name, YAML::Node config) :
			RobotCommand(name) {
		flapperSpeed = config["flapperSpeed"].as<double>();
	}

	static std::string GetBaseName() {
		return "IntakeTotes";
	}

protected:
	void Execute() override {
		robot->toteFeed->SetFlapperSpeed(flapperSpeed);
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		RobotCommand::End();
		robot->toteFeed->SetFlapperSpeed(0);
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		robot->toteFeed->SetFlapperSpeed(0);
	}

private:
	double flapperSpeed;
};

}

#endif /* INTAKETOTES_H_ */
