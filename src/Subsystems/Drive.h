/**
 * @file Drive.h
 * @author Daniele Moro
 * @date Jan 21, 2015
 */

#ifndef DRIVE_H_
#define DRIVE_H_

#include <Common/SubsystemBase.h>
#include <yaml-cpp/yaml.h>

namespace tator {

/**
 * Drives robot for the glory of Russia
 */
class Drive: public SubsystemBase {
private:
	Talon* driveL;
	Talon* driveR;
	Encoder* encoderL;
	Encoder* encoderR;
	PIDController* pidL;
	PIDController* pidR;

public:
	Drive(YAML::Node& config);
	virtual ~Drive();

	/**
	 * Sets the speed of the  wheels in
	 * 1st value is left speed, 2nd value is right speed
	 * @param speed
	 */
	void SetSpeeds(float leftSpeed, float rightSpeed);

};

} /* namespace tator */

#endif /* DRIVE_H_ */
