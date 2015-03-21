#ifndef ROLLERBEDPISTON_H_
#define ROLLERBEDPISTON_H_

#include "CommandBase.h"
#include "Subsystems/ToteFeed.h"

namespace tator {

class RollerbedPiston: public CommandBase {
public:
	RollerbedPiston(std::string name, YAML::Node config) :
			CommandBase(name) {
		targetState =
				(ToteFeed::RollerPistonState) config["direction"].as<int>();
	}

	static std::string GetBaseName() {
		return "RollerbedPiston";
	}

protected:
	void Execute() override {
		toteFeed->SetRollerPiston(targetState);
	}

	bool IsFinished() override {
		return true;
	}

private:
	ToteFeed::RollerPistonState targetState;
};

}

#endif /* ROLLERBEDPISTON_H_ */
