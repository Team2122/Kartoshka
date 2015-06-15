#ifndef CLAWSMARTROLLERS_H
#define CLAWSMARTROLLERS_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawSmartRollers: public CommandBase {
public:
	COMMAND_METHODS(ClawSmartRollers);

private:
	double waitTime;
	bool isWaiting;
	Timer timer;
};

}

#endif /* CLAWSMARTROLLERS_H */
