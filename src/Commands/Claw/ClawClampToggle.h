/**
 * @file ClawClampToggle.h
 * @author Alex Mikhalev
 * @date 3/8/2015
 */

#ifndef CLAWCLAMPTOGGLE_H_
#define CLAWCLAMPTOGGLE_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawClampToggle: public CommandBase {
public:
	COMMAND_BASE_METHODS(ClawClampToggle);
};

}

#endif /* CLAWCLAMPTOGGLE_H_ */
