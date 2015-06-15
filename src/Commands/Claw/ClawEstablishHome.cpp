#include "ClawEstablishHome.h"

namespace tator {

COMMAND_CONSTRUCTOR_DEF(ClawEstablishHome) {
	hasEstablished = false;
}

void ClawEstablishHome::Execute() {
	// Claw _not_ at home
	if (!claw->IsAtHome()) {
		// Only establish home once
		if (!hasEstablished) {
			DriverStation* driverStation = DriverStation::GetInstance();
			// If we are connected to FMS
			if (driverStation->IsFMSAttached()) {
				// Just output a warning
				log.Warn("Claw was not homed at the start of auto!");
				// And let the claw work
				claw->ZeroLiftEncoder();
			}
			// If we're just running tele (ie. at practice)
			else {
				// Output an error
				log.Error("Claw was not homed before enabling."
						" Please home the claw and restart"
						" the robot code.");
				// And kill the claw
				claw->DisableClaw();
			}
		}
	}
	// Claw is home
	else {
		// All good, reset the lift encoder
		claw->ZeroLiftEncoder();
	}
	hasEstablished = true;
}

bool ClawEstablishHome::IsFinished() {
	return true;
}

}
