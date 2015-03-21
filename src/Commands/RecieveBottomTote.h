/**
 * @file RecieveBottomTote.h
 * @date Jan 24, 2015
 * @author Lee Bousfield
 */

#include "Subsystems/ToteFeed.h"
#include "CommandBase.h"

#ifndef RECIEVETOTE_H_
#define RECIEVETOTE_H_

namespace tator {

class RecieveBottomTote: public CommandBase {
public:
	RecieveBottomTote(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(toteFeed);
		flapperSpeed = config["flapperSpeed"].as<double>();
		rollerSpeed = config["rollerSpeed"].as<double>();
	}

	static std::string GetBaseName() {
		return "RecieveBottomTote";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		toteFeed->SetFlapperSpeed(flapperSpeed);
		toteFeed->SetRollers(rollerSpeed);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return toteFeed->GetBackSensor();
	}

	void End() override {
		log.Info("Back beam triggered, stopping rollers");
		CommandBase::End();
		toteFeed->SetFlapperSpeed(0);
		toteFeed->SetRollers(0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		toteFeed->SetFlapperSpeed(0);
		toteFeed->SetRollers(0);
	}

private:
	double flapperSpeed;
	double rollerSpeed;
};

}

#endif /* RECIEVETOTE_H_ */
