/**
 * @file Claw.h
 * @author Paul Vaughan
 * @date Jan 19, 2015
 */

#ifndef CLAW_H
#define CLAW_H

#include <WPILib.h>
#include "Common/SubsystemBase.h"
#include "Common/Config.h"

namespace tator {

/**
 * Dis claw constrict container with great force
 * and let go of containment very gently like similar to mother
 */
class Claw: public SubsystemBase {
private:
	DoubleSolenoid* clampOne;
	DoubleSolenoid* clampTwo;
	DoubleSolenoid* finger;
	Talon* rollers;
	DigitalInput* button;

	double rollerInwardSpeed;
	double rollerOutwardSpeed;

public:
	Claw(YAML::Node config);
	~Claw();

	enum ClawRollerStatus {
		kOutward, kInward
	};
	/**
	 * Da roller speed is become in, out, or oof.
	 */
	void SetRollerSpeed(ClawRollerStatus operation);
	/**
	 * Puny container is crushed from Mother Russia's technologicaly superior claw.
	 */
	void ClampContainer();
	/**
	 * De motherland show mercy to container
	 */
	void ReleaseContainer();
	/**
	 * The motherland demand robot to say if it has shown mercy to puny
	 */
	bool HasContainer();
};

}

#endif
