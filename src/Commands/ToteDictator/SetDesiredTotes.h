/**
 * @file SetDesiredTotes.h
 * @author Zachary Snow
 * @date Mar 20, 2015
 */

#ifndef SETDESIREDTOTES_H_
#define SETDESIREDTOTES_H_

#include "Robot.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class SetDesiredTotes: public RobotCommand {
public:
	SetDesiredTotes(std::string name, YAML::Node config) :
			RobotCommand(name) {
		number = config["number"].as<int>();
	}

	static std::string GetBaseName() {
		return "SetDesiredTotes";
	}

protected:
	void Execute() override {
		robot->shuttle->SetDesiredTotes(number);
	}

	bool IsFinished() override {
		return true;
	}

private:
	int number;
};

}

#endif /* SETDESIREDTOTES_H_ */
