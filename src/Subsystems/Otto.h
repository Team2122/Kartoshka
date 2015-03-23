/**
 * @file Otto.h
 * @author Nick Hodes
 * @date Jan 29, 2015
 */

#ifndef OTTO_H
#define OTTO_H

#include <WPILib.h>
#include "Common/SubsystemBase.h"
#include "Common/Config/Config.h"
#include "Common/Hardware/ADXRS453.h"

namespace tator {

class Otto: public SubsystemBase {
private:
	DigitalInput *autoSwitch0; ///< The first switch for auto
	DigitalInput *autoSwitch1; ///< The second switch for auto
	DigitalInput *autoSwitch2; ///< The third switch for auto

	ADXRS453* gyro; ///< Wonderful communist gyro

public:
	/**
	 * The constructor for Otto
	 */
	Otto(YAML::Node config);

	/**
	 * The destructor for Otto
	 */
	~Otto();

	/**
	 * Gets the auto mode number
	 * @return the number of the auto mode (0 to 7)
	 */
	int GetAutoModeNumber();

	/**
	 * Starts the auto command based off of the GetAutoModeNumber()
	 */
	void StartAutoCommand();

	/**
	 * Gets how far from communism you have turned
	 * @return How far from communism in degrees
	 */
	double GetAngle();

	/**
	 * Resets angle to straight communsim
	 */
	void ResetAngle();

	/**
	 * Makes sure communism sensor is calibrated (zerod)
	 */
	void StartGyroCalibration();

	/**
	 * Forces gyro to be calibrated or else...
	 */
	void FinishGyroCalibration();

};

} /* namespace tator */

#endif /* OTTO_H */
