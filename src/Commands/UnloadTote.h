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

protected:
	void Initialize() override {
		CommandBase::Initialize();
		toteFeed->SetRollers(rollerSpeed);
		toteFeed->SetFlapperSpeed(flapperSpeed);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		CommandBase::End();
		toteFeed->SetRollers(0);
		toteFeed->SetFlapperSpeed(0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		toteFeed->SetRollers(0);
		toteFeed->SetFlapperSpeed(0);
	}

private:
	double rollerSpeed, flapperSpeed;
};

}

#endif /* UNLOADTOTE_H_ */
