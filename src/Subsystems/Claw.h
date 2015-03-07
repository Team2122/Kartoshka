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
	Talon* liftMotor; ///> Talon for the Vertical, Incremental
	Talon* clawRotation; ///> Talon for the Rotation, Absolute
	Encoder* liftEncoder; ///> Encoder for the Vertical
	AnalogPotentiometer* rotationAngle; ///> Encoder for the Rotation
	DigitalInput* upperLimit;
	DigitalInput* homeLimit;
	PowerDistributionPanel* PDP;

	double clearClawRotate; ///> Minimum safety for claw to fully rotate
	double clearClawMinAngle; ///> Minimum safety for claw to fully rotate
	double clearClawMinHeight;

	float upSpeed;
	float downSpeed;

	Solenoid* clampLong;
	Solenoid* clampShort;
	Talon* rollers;
	DigitalInput* binSensor;

	double rollerInwardSpeed;
	double rollerOutwardSpeed;

	bool disabled;

	ClawAngle clawAngle;
	bool rotationFinished;
	LiftSpeed liftSpeed;

public:
	Claw(YAML::Node config);
	~Claw();

	/**
	 * Disables the claw until the code is rebooted
	 */
	void DisableClaw();

	void ReenableClaw();

	/**
	 * Sets the power of the lift
	 * @param power The amount of power to power
	 */
	void SetVerticalLiftMotor(double power);

	void SetLiftSpeed(LiftSpeed speed);

	LiftSpeed GetLiftSpeed();

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

	void SetRotationSpeed(double speed, bool override = false);

	void SetRotationFinished(bool rotationFinished);

	bool IsRotationFinished();

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

	double GetCurrent(int channel);
};

}

#endif
