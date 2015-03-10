/**
 * @file FlapFlappers.h
 * @date Feb 12, 2015
 * @author Lee Bousfield
 */

#ifndef FLAPFLAPPERS_H_
#define FLAPFLAPPERS_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"
#include "Subsystems/ToteFeed.h"

namespace tator {

class FlapFlappers: public CommandBase {
public:
	FlapFlappers(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "FlapFlappers";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		toteFeed->SetFlapperSpeed(true);
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return shuttle->IsTotePresent();
	}

	void End() override {
		CommandBase::End();
		toteFeed->SetFlapperSpeed(false);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		toteFeed->SetFlapperSpeed(false);
	}
};

}

#endif /* FLAPFLAPPERS_H_ */

