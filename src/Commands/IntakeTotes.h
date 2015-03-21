/**
 * @file IntakeTotes.h
 * @date Mar 20, 2015
 * @author Zachary Snow
 */

#include "Subsystems/ToteFeed.h"
#include "CommandBase.h"

#ifndef INTAKETOTES_H_
#define INTAKETOTES_H_

namespace tator {

class IntakeTotes: public CommandBase {
public:
	IntakeTotes(std::string name, YAML::Node config) :
			CommandBase(name) {
		flapperSpeed = config["flapperSpeed"].as<double>();
	}

	static std::string GetBaseName() {
		return "IntakeTotes";
	}

protected:
	void Execute() override {
		toteFeed->SetFlapperSpeed(flapperSpeed);
	}

	bool IsFinished() override {
		return false;
	}

	void End() override {
		CommandBase::End();
		toteFeed->SetFlapperSpeed(0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		toteFeed->SetFlapperSpeed(0);
	}

private:
	double flapperSpeed;
};

}

#endif /* INTAKETOTES_H_ */
