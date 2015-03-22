/**
 * @file SetDesiredTotes.h
 * @author Zachary Snow
 * @date Mar 20, 2015
 */

#ifndef SETDESIREDTOTES_H_
#define SETDESIREDTOTES_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class SetDesiredTotes: public CommandBase {
public:
	SetDesiredTotes(std::string name, YAML::Node config) :
			CommandBase(name) {
		number = config["number"].as<int>();
	}

	static std::string GetBaseName() {
		return "SetDesiredTotes";
	}

protected:
	void Execute() override {
		shuttle->SetDesiredTotes(number);
	}

	bool IsFinished() override {
		return true;
	}

private:
	int number;
};

}

#endif /* SETDESIREDTOTES_H_ */
