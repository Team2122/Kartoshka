#ifndef CLAWESTABLISHHOME_H_
#define CLAWESTABLISHHOME_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include <DriverStation.h>

namespace tator {

class ClawEstablishHome: public CommandBase {
public:
	COMMAND_BASE_METHODS(ClawEstablishHome);

private:
	bool hasEstablished;
};

}

#endif
