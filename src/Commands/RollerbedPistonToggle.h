#ifndef ROLLERBEDPISTONTOGGLE_H_
#define ROLLERBEDPISTONTOGGLE_H_

#include "CommandBase.h"
#include "Subsystems/ToteFeed.h"

namespace tator {

class RollerbedPistonToggle: public CommandBase {
public:
	RollerbedPistonToggle(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "RollerbedPistonToggle";
	}

protected:
	void Execute() override {
		if (toteFeed->GetRollerPiston()) {
			toteFeed->SetRollerPiston(ToteFeed::RollerPistonState::down);
		} else {
			toteFeed->SetRollerPiston(ToteFeed::RollerPistonState::up);
		}
	}

	bool IsFinished() {
		return true;
	}
};

}

#endif /* ROLLERBEDPISTONTOGGLE_H_ */
