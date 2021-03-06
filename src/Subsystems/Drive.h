/**
 * @file Drive.h
 * @author Daniele Moro
 * @date Jan 21, 2015
 */

#ifndef DRIVE_H_
#define DRIVE_H_

#include <Talon.h>
#include <Encoder.h>
#include <PIDController.h>
#include "Common/SubsystemBase.h"
#include "Common/Config/Config.h"

namespace tator {

/**
 * Drives robot for the glory of Russia
 */
class Drive: public SubsystemBase {
	friend class Tester;

public:
	enum class Mode {
		pid, direct
	};

private:
	Talon* driveL; ///< The left drive talon
	Talon* driveR; ///< The right drive talon
	Encoder* encoderL; ///< The encoder for the left drive
	Encoder* encoderR; ///< The encoder for the right drive
	PIDController* pidL; ///< The PID for the left drive
	PIDController* pidR; ///< The PID for the right drive
	double maxRPS; ///< The maximum Rotations Per Second for the drive
	Mode mode; ///< The current speed control mode

public:
	Drive(YAML::Node config);
	virtual ~Drive();

	/**
	 * Sets the speed of the  wheels
	 * 1st value is left speed, 2nd value is right speed
	 * @param leftSpeed The speed from -1 to 1 for left wheels
	 * @param rightSpeed The speed from -1 to 1 for right wheels
	 */
	void SetSpeeds(float leftSpeed, float rightSpeed);

	/**
	 * Sets the method for velocity control
	 * @param mode The desired mode of control
	 */
	void SetControlMode(Mode mode);

	/**
	 * Gets the current distance traveled in feet
	 * @return Distance in feet
	 */
	double GetDistance();
};

} /* namespace tator */

#endif /* DRIVE_H_ */
