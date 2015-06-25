/*
 * @file ClawHomeTop.h
 * @author Alex Mikhalev
 */

#ifndef CLAWHOMETOP_H_
#define CLAWHOMETOP_H_

#include "CommandBase.h"

namespace tator {

class ClawHomeTop: public CommandBase {
public:
	ClawHomeTop(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(claw);

		speed = config["speed"].as<double>();
	}

	static std::string GetBaseName() {
		return "ClawHomeTop";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		if (claw->IsHomed()) {
			log.Error("Claw is already homed!");
			this->Cancel();
		} else {
			log.Info("Homing the claw at the top");
		}
	}

	void Execute() override {
		claw->ForceSetLiftSpeed(speed);
	}

	bool IsFinished() override {
		return claw->IsAtTop();
	}

	void End() override {
		CommandBase::End();
		claw->SetLiftSpeed(0);
		claw->Home();
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		claw->SetLiftSpeed(0);
	}

private:
	double speed;
};

}

#endif /* CLAWHOMETOP_H_ */
