/**
 * @file ClawClamp.h
 * @author Paul Vaughan
 */

#ifndef CLAWCLAMP_H
#define CLAWCLAMP_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawClamp: public CommandBase {
public:
	COMMAND_BASE_METHODS(ClawClamp);

private:
	Claw::ClampStatus clampStatus;
};

}

#endif
