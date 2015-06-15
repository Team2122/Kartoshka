/**
 * @file ClawToAngle.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef CLAWTOANGLE_H_
#define CLAWTOANGLE_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"

namespace tator {

class ClawToAngle: public CommandBase {
public:
	COMMAND_METHODS(ClawToAngle);

private:
	Claw::ClawAngle angle;
	double speed, holdSpeed;
	double rampDistance, rampFactor;
	double minimumHeight;
};

}

#endif /* CLAWTOANGLE_H_ */
