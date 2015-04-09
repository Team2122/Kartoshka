#ifndef CLAWESTABLISHHOME_H_
#define CLAWESTABLISHHOME_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include <DriverStation.h>

namespace tator {

class ClawEstablishHome: public CommandBase {
public:
	ClawEstablishHome(std::string name, YAML::Node config) :
			CommandBase(name) {
		hasEstablished = false;
	}

	static std::string GetBaseName() {
		return "ClawEstablishHome";
	}

protected:
	void Execute() override {
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

	bool IsFinished() override {
		return true;
	}

private:
	bool hasEstablished;
};

}

#endif
