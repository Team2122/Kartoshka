/**
 * @file HoldBottomTote.h
 * @date Mar 20, 2015
 * @author Zachary Snow
 */

#include "Subsystems/ToteFeed.h"
#include "CommandBase.h"

#ifndef HOLDBOTTOMTOTE_H_
#define HOLDBOTTOMTOTE_H_

namespace tator {

class HoldBottomTote: public CommandBase {
public:
	HoldBottomTote(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(toteFeed);
		rollerSpeed = config["rollerSpeed"].as<double>();
	}

	static std::string GetBaseName() {
		return "HoldBottomTote";
	}

protected:
	void Execute() override {
		toteFeed->SetRollers(rollerSpeed);
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
	double rollerSpeed;
};

}

#endif /* HOLDBOTTOMTOTE_H_ */
