/**
 * @file UpdateTotesRatcheted.h
 * @author Alex Mikhalev
 * @date 2/14/2015
 */

#ifndef UPDATETOTESRATCHETED_H_
#define UPDATETOTESRATCHETED_H_

#include "Robot.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class UpdateTotesRatcheted: public RobotCommand {
public:
	UpdateTotesRatcheted(std::string name, YAML::Node config) :
			RobotCommand(name) {
	}

	static std::string GetBaseName() {
		return "UpdateTotesRatcheted";
	}

protected:
	void Execute() override {
		robot->shuttle->UpdateTotesRatcheted();
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif /* UPDATETOTESRATCHETED_H_ */
