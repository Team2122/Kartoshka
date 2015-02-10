/**
 * @file Claw.h
 * @author Paul Vaughan
 * @date Jan 24, 2015
 */

#ifndef CLAW_H
#define CLAW_H

#include <WPILib.h>
#include "Common/SubsystemBase.h"
#include "Common/Config.h"

namespace tator {

/**
 * De lift inspire claw to move both up end not up
 */
class Claw: public SubsystemBase {
private:
	Talon* liftVertical; ///> Talon for the Vertical, Incremental
	Talon* clawRotation; ///> Talon for the Rotation, Absolute
	Encoder* verticalTicks; ///> Encoder for the Vertical
	AnalogPotentiometer* rotationAngle; ///> Encoder for the Rotation

	float maxLiftAngle; ///> maximum safety for the rotation
	float minLiftAngle; ///> minimum safety for the rotation
	float maxLiftHeight; ///> maximum safety for the vertical
	float minLiftHeight; ///> minimum safety for the vertical
	float clearClawMax; ///> maximum value for claw being able to move
	float clearClawMin; ///> minimum value for claw being able to move
	float backwardRotationSpeed;
	float forwardRotationSpeed;

	DoubleSolenoid* clampOne;
	DoubleSolenoid* clampTwo;
	Talon* rollers;
	DigitalInput* button; // now a sensor, not a button

	double rollerInwardSpeed;
	double rollerOutwardSpeed;

public:
	Claw(YAML::Node config);
	~Claw();

	/**
	 * Changes the lift from its current height to a new height
	 */
	void SetPosition(float newHeight);

	/**
	 * Returns the current height of the robot
	 */
	double GetPosition();

	/**
	 * Resets the encoder to the value of 0
	 */
	void ResetTicks();

	/**
	 * Return the rotation angle of the lift
	 */
	double GetAngle();

	///////////////////////////////////////////////////////////////

	enum ClawRollerStatus {
		kOutward, kInward
	};

	enum ClawClampStatus {
		kLong = 3, kMid = 2, kShort = 1, kNone = 0
	//the longer the piston is, the more closed the claw becomes
	};

	/**
	 * Da roller speed is become in, out, or oof.
	 */
	void SetRollerSpeed(ClawRollerStatus operation);

	/**
	 * Puny container is crushed from Mother Russia's technologicaly superior claw.
	 */
	void SetContainerClampStatus(ClawClampStatus extendTo);

	/**
	 * Puny container is crushed from Mother Russia's technologicaly superior claw.
	 */
	ClawClampStatus GetContainerClampStatus();

	enum RotationDirection {
		kBackward, kForward, kStopped
	};

	void SetRotationDirection(RotationDirection dir);

	float GetRoatationAngle();

	/**
	 * The motherland demand robot to say if it has shown mercy to puny
	 */
	bool HasContainer();
};

}

#endif
