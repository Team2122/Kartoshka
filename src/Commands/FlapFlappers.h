/**
 * @file FlapFlappers.h
 * @date Feb 12, 2015
 * @author Lee Bousfield
 */

#include "CommandBase.h"

#ifndef FLAPFLAPPERS_H_
#define FLAPFLAPPERS_H_

namespace tator {

class FlapFlappers: public CommandBase {
public:
	FlapFlappers(std::string name, YAML::Node config):
			CommandBase(name) {
	}
	void Initialize() {
		toteFeed->SetFlappers(true);
	}
	void Execute() {
	}
	bool IsFinished() {
		return shuttle->IsTotePresent();
	}
	void End() {
		toteFeed->SetFlappers(false);
	}
	void Interrupted() {
		toteFeed->SetFlappers(false);
	}
	static std::string GetBaseName() {
		return "FlapFlappers";
	}
};

}

#endif /* FLAPFLAPPERS_H_ */

