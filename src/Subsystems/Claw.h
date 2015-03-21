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
#include "Common/FixedField.h"

namespace tator {

/**
 * De lift inspire claw to move both up end not up
 */
class Claw: public SubsystemBase {
public:
	enum class RotationSpeed {
		kStopped = 0, kBackward, kForward, kHoldPick, kUnknown
	};

	enum class LiftSpeed {
		kStop = 0, kUp = 1, kDown = 2
	};

	enum class ClawAngle {
		kPick = 0, kPlatform = 1
	};
private:
	Talon* liftMotor; ///< Talon for the Vertical, Incremental
	Talon* clawRotation; ///< Talon for the Rotation, Absolute
	Encoder* liftEncoder; ///< Encoder for the Vertical
	AnalogPotentiometer* rotationAngle; ///< Encoder for the Rotation
	FixedField* upperLimit; ///< The upper limit sensor of the claw
	FixedField* homeLimit; ///< The lower/home limit sensor of the claw

	double clearClawRotate; ///< Minimum safety for claw to fully rotate
	double clearClawMinAngle; ///< Minimum safety for claw to fully rotate
	double clearClawMinHeight; ///< Minimum safety for claw to move up and down

	float upSpeed; ///< How fast the claw will move up
	float downSpeed; ///< How fast the claw will move down

	Solenoid* clampLong; ///< The solenoid for the long clamp
	Solenoid* clampShort; ///< The solenoid for the short clamp
	Talon* rollers; ///< The roller motor
	FixedField* binSensor; ///< The sensor that senses all the bins

	double rollerInwardSpeed; ///< The inward speed of the rollers
	double rollerOutwardSpeed; ///< The outward speed of the rollers

	bool disabled; ///< Checks to see if the robot is disabled

	ClawAngle clawAngle; ///< Gets the claw angle
	bool rotationFinished; ///< Checks if the rotation is finished
	LiftSpeed liftSpeed; ///< The lift speed of the claw

public:
	Claw(YAML::Node config);
	~Claw();

	/**
	 * Disables the claw until the code is rebooted
	 */
	void DisableClaw();

	/**
	 * Reenables the claw
	 */
	void ReenableClaw();

	/**
	 * Sets the lift speed to power if not disabled
	 * @param power, the power the motor will be set to
	 */
	void SetLiftSpeed(double power);

	/**
	 * Sets the lift speed to speed
	 * @param speed, the speed of the robot
	 */
	void SetLiftSpeed(LiftSpeed speed);

	/**
	 * Gets the lift speed of the claw
	 * @return the LiftSpeed
	 */
	LiftSpeed GetLiftSpeed();

	/**
	 * Gets the lift speed of the claw
	 * @param speed, the lift speed of the claw
	 * @return the lift speed
	 */
	double GetLiftSpeed(LiftSpeed speed);

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

	/**
	 * Sets the rotation speed of the claw
	 * @param speed, the speed the claw will rotate at
	 * @param override, set to false unless told otherwise
	 */
	void SetRotationSpeed(double speed, bool override = false);

	/**
	 * Sets the rotationFinished variable to the parameter given
	 * @param rotationFinished, what the rotationFinished will be set to
	 */
	void SetRotationFinished(bool rotationFinished);

	/**
	 * Returns true or false depending on if the rotation is finished
	 * @return rotationFinished
	 */
	bool IsRotationFinished();

	/**
	 * Gets the rotation angle
	 * @return rotationAngle
	 */
	float GetRotationAngle();

	/**
	 * The motherland demand robot to say if it has shown mercy to puny
	 */
	bool HasContainer();

	/**
	 * Mother Russia shows gentle side end politely esk for TargetAngle
	 */
	ClawAngle GetTargetAngle();

	/**
	 * KGB invade and persuade target angle to change ideas
	 */
	void SetTargetAngle(ClawAngle newClawAngle);
};

}

#endif
