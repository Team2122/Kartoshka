/**
 * @file RecieveTote.h
 * @date Jan 24, 2015
 * @author Lee Bousfield
 */

#include "Subsystems/ToteFeed.h"
#include "CommandBase.h"

#ifndef UNLOADTOTE_H_
#define UNLOADTOTE_H_

namespace tator {

class UnloadTote: public CommandBase {
public:
	UnloadTote(std::string name, YAML::Node config) :
			CommandBase(name) {
		rollerSpeed = config["rollerSpeed"].as<double>();
		flapperSpeed = config["flapperSpeed"].as<double>();
	}

	static std::string GetBaseName() {
		return "UnloadTote";
	}

	void Initialize() {
		CommandBase::Initialize();
		toteFeed->SetRollers(rollerSpeed);
		toteFeed->SetFlapperSpeed(flapperSpeed);
	}

	void Execute() {
	}

	bool IsFinished() {
		return false;
	}

	void End() {
		CommandBase::End();
		toteFeed->SetRollers(0);
		toteFeed->SetFlapperSpeed(0);
	}

	void Interrupted() {
		CommandBase::Interrupted();
		toteFeed->SetRollers(0);
		toteFeed->SetFlapperSpeed(0);
	}
private:
	double rollerSpeed, flapperSpeed;
};

}

#endif /* UNLOADTOTE_H_ */
