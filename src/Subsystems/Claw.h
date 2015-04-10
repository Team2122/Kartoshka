/**
 * @file Claw.h
 * @author Paul Vaughan
 * @date Jan 24, 2015
 */

#ifndef CLAW_H
#define CLAW_H

#include <Talon.h>
#include <Encoder.h>
#include <Solenoid.h>

#ifndef ANALOGPOTENTIOMETER_H_
#define ANALOGPOTENTIOMETER_H_
#include <AnalogPotentiometer.h>
#endif

#include "Common/SubsystemBase.h"
#include "Common/Config/Config.h"
#include "Common/Hardware/FixedField.h"

namespace tator {

/**
 * De lift inspire claw to move both up end not up
 */
class Claw: public SubsystemBase {
public:
	enum class ClawAngle {
		neither, front, back
	};

	//kLeft and kRight refer to the piston that is currently opened
	//if kLeft, then the left side will be in, ready to pick
	enum class ClampStatus {
		kReleased = 0, kLeft = 1, kRight = 2, kDeathGrip = 3
	};

	enum class RollerStatus {
		kStopped = 0, kOutward = 1, kInward = 2
	};

private:
	bool enabled; ///< Whether or not the claw is in a known position

	Talon* liftMotor; ///< Talon for the vertical
	Encoder* liftEncoder; ///< Encoder for the vertical, incremental
	double heightTolerance; ///< The tolerance that the claw height must be within

	FixedField* homeLimit; ///< The lower/home limit sensor of the claw
	FixedField* topLimit; ///< The upper limit sensor of the claw

	Talon* rotationMotor; ///< Talon for the rotation
	AnalogPotentiometer* rotationEncoder; ///< Encoder for the rotation, absolute
	double angleTolerance; ///< The tolerance that the claw angle must be within
	double frontAngle; ///< The angle when the claw is in the front
	double backAngle; ///< The angle when the claw is in the back

	Solenoid* clampLeft; ///< The solenoid for the left clamp
	Solenoid* clampRight; ///< The solenoid for the right clamp

	Talon* rollers; ///< The roller motor
	double rollerInwardSpeed; ///< The inward speed of the rollers
	double rollerOutwardSpeed; ///< The outward speed of the rollers

	FixedField* binSensor; ///< The sensor that senses all the bins

public:
	Claw(YAML::Node config);
	~Claw();

	/**
	 * Disables the claw until the code is rebooted. Prevents the claw from
	 * moving up or down or rotating.
	 */
	void DisableClaw();

	/**
	 * Enables the claw after being disabled.
	 */
	void EnableClaw();

	/**
	 * Sets the lift speed to power if not disabled
	 * @param speed, the power the motor will be set to
	 */
	void SetLiftSpeed(double speed);

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
	 * Checks if the claw lift is within the specified height
	 * @param height The height to check
	 * @return true if it is within the tolerance
	 */
	bool IsAtHeight(double height);

	/**
	 * Checks if the home limit is tripped
	 * @return If the claw lift is home
	 */
	bool IsAtHome();

	/**
	 * Checks if the top limit is tripped
	 * @return If the claw lift is at the very top
	 */
	bool IsAtTop();

	/**
	 * Converts the name of a claw angle to the actual angle
	 * @param name The name of the angle
	 * @return The actual angle
	 */
	ClawAngle AngleFromName(std::string name);

	/**
	 * Checks if the claw is at the specified angle
	 * @param angle The angle to check if the claw is at
	 * @return True if it is at the angle
	 */
	bool IsAtAngle(ClawAngle angle);

	/**
	 * Sets the rotation speed of the claw
	 * @param speed, the speed the claw will rotate at
	 */
	void SetRotationSpeed(double speed);

	/**
	 * @return How many degrees it is away from the angle
	 */
	double GetDegreesFromAngle(ClawAngle angle);

	/**
	 * Da roller speed is become in, out, or oof.
	 */
	void SetRollerSpeed(RollerStatus operation);

	/**
	 * Puny container is crushed from Mother Russia's technologically superior claw.
	 */
	void SetClampStatus(ClampStatus status);

	/**
	 * Interrogates claw pistons about clamp status
	 * @return The clamp status
	 */
	ClampStatus GetClampStatus();

	/**
	 * The motherland demand robot to say if it has shown mercy to puny
	 */
	bool HasContainer();
};

}

#endif
