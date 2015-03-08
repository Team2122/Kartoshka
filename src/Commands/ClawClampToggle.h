/**
 * @file ClawClampToggle.h
 * @author Alex Mikhalev
 * @date 3/8/2015
 */

#ifndef CLAWCLAMPTOGGLE_H_
#define CLAWCLAMPTOGGLE_H_

#include "CommandBase.h"

namespace tator {

class ClawClampToggle: public CommandBase {
public:
	ClawClampToggle(std::string name, YAML::Node config) :
			CommandBase(name) {

	}

	static std::string GetBaseName() {
		return "ClawClampToggle";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		claw->SetClampStatus(Claw::ClampStatus::kReleased);
	}
	void Execute() override {

	}
	bool IsFinished() override {
		return false;
	}
	void End() override {
		CommandBase::End();
		claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
	}
	void Interrupted() override {
		CommandBase::Interrupted();
		claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
	}
};

}
;

#endif /* CLAWCLAMPTOGGLE_H_ */
