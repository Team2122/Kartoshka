/**
 * @file HoldBottomTote.h
 * @date Mar 20, 2015
 * @author Zachary Snow
 */

#ifndef HOLDBOTTOMTOTE_H_
#define HOLDBOTTOMTOTE_H_

#include "Subsystems/ToteFeed.h"
#include "CommandBase.h"

namespace tator {

class HoldBottomTote: public CommandBase {
public:
	HoldBottomTote(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(toteFeed);
		rollerSpeedSlow = config["rollerSpeed"]["slow"].as<double>();
		rollerSpeedFast = config["rollerSpeed"]["fast"].as<double>();
	}

	static std::string GetBaseName() {
		return "HoldBottomTote";
	}

protected:
	void Execute() override {
		if (toteFeed->GetBackSensor()) {
			toteFeed->SetRollers(rollerSpeedSlow);
		} else {
			toteFeed->SetRollers(rollerSpeedFast);
		}
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		CommandBase::End();
		toteFeed->SetRollers(0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		toteFeed->SetRollers(0);
	}

private:
	double rollerSpeedSlow;
	double rollerSpeedFast;
};

}

#endif /* HOLDBOTTOMTOTE_H_ */
