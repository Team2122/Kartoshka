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
	Encoder* liftEncoder; ///> Encoder for the vertical lift, relative
	Talon* clawRotation; ///> Talon for the Rotation, Absolute
	Encoder* verticalTicks; ///> Encoder for the Vertical
	AnalogPotentiometer* rotationAngle; ///> Encoder for the Rotation
	DigitalInput* upperLimit;
	DigitalInput* homeLimit;

	float maxLiftAngle; ///> maximum safety for the rotation
	float minLiftAngle; ///> minimum safety for the rotation
	float maxLiftHeight; ///> maximum safety for the vertical
	float minLiftHeight; ///> minimum safety for the vertical
	float clearClawMax; ///> maximum value for claw being able to move
	float clearClawMin; ///> minimum value for claw being able to move
	float backwardRotationSpeed;
	float forwardRotationSpeed;

	float upSpeed;
	float downSpeed;

	Solenoid* clampLong;
	Solenoid* clampShort;
	Talon* rollers;
	DigitalInput* binSensor;

	double rollerInwardSpeed;
	double rollerOutwardSpeed;

	bool disabled;

public:
	Claw(YAML::Node config);
	~Claw();

	/**
	 * Disables the claw until the code is rebooted
	 */
	void DisableClaw();

	/**
	 * Sets the power of the lift
	 * @param power The amount of power to power
	 */
	void SetVerticalLiftMotor(double power);

	enum class LiftSpeed {
		kStop, kUp, kDown
	};

	void SetLiftSpeed(LiftSpeed speed);

	/**
	 * Gets the position of the lift encoder
	 * @return The position of the lift encoder
	 */
	double GetLiftEncoder();

	/**
	 * Zeros the lift encoder
	 */
	void ZeroLiftEncoder();

	/**
	 * Returns the current height of the robot
	 */
	int32_t GetPosition();

	/**
	 * Checks if the home limit is tripped
	 * @return If the claw/lift is home
	 */
	bool IsHome();

	/**
	 * Checks if the top limit is tripped
	 * @return If the claw/lift is at the very top
	 */
	bool IsTop();

	/**
	 * Resets the encoder to the value of 0
	 */
	void ResetTicks();

	///////////////////////////////////////////////////////////////

	enum class RollerStatus {
		kStopped = 0, kOutward = 1, kInward = 2
	};

	/**
	 * Da roller speed is become in, out, or oof.
	 */
	void SetRollerSpeed(RollerStatus operation);

	enum class ClampStatus {
		kReleased = 0, kTote = 1, kContainer = 2, kDeathGrip = 3
	};

	/**
	 * Puny container is crushed from Mother Russia's technologically superior claw.
	 */
	void SetClampStatus(ClampStatus status);

	enum class RotationSpeed {
		kStopped = 0, kBackward, kForward
	};

	void SetRotationSpeed(RotationSpeed dir);

	float GetRotationAngle();

	/**
	 * The motherland demand robot to say if it has shown mercy to puny
	 */
	bool HasContainer();
};

}

#endif
