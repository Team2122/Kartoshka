/**
 * @file ResetMaxTotes.h
 * @author Alex Mikhalev
 * @date 3/8/2015
 */

#ifndef RESETMAXTOTES_H_
#define RESETMAXTOTES_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ResetMaxTotes: public CommandBase {
public:
	ResetMaxTotes(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "ResetMaxTotes";
	}

protected:
	void Execute() override {
		shuttle->ResetMaxToteCount();
		log.Info("Will now make a %d high stack",
				shuttle->GetMaxToteCount() + 2);
	}

	bool IsFinished() override {
		return true;
	}
};

}

#endif /* RESETMAXTOTES_H_ */
