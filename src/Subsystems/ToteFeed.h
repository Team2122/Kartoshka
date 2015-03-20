/**
 * @file ToteFeed.h
 * @author Lee Bousfield
 * @date Jan 24, 2015
 */

#ifndef TOTEFEED_H_
#define TOTEFEED_H_

#include <WPILib.h>
#include "Common/SubsystemBase.h"
#include <yaml-cpp/yaml.h>

namespace tator {

/**
 * For the ToteFeed/Conveyor
 * Moves totes
 */
class ToteFeed: public SubsystemBase {
public:
	enum class RollerPistonState {
		up = 1, down = 0
	};

	/**
	 * Inits a tote feed
	 * @param config the config for the subsystem
	 */
	ToteFeed(YAML::Node config);
	/**
	 * Deconstructs a tote feed
	 */
	~ToteFeed();
	/**
	 * Gets the back beam sensor
	 * @return the back beam sensor
	 */
	bool GetBackSensor();
	/**
	 * Sets the roller speed
	 * @param speed the new speed of the rollers
	 */
	void SetRollers(double speed);
	/**
	 * Sets the roller piston state
	 * @param state the state to change it to
	 */
	void SetRollerPiston(RollerPistonState state);
	/**
	 * Gets the roller piston state
	 * @return the state of the roller piston
	 */
	bool GetRollerPiston();
	/**
	 * Sets the flappers
	 * @param flappers the speed of the flappers
	 */
	void SetFlapperSpeed(float speed);

private:
	Talon* rollers; ///< The roller motors
	Talon* flappers; ///< The flapper motors
	DigitalInput* backSensor; ///< The back sensor
	Solenoid* rollerPiston; ///< The solenoid for the tote feed rollers
	double flapperSpeed; ///< Sets the flapper speed
};

}

#endif /* TOTEFEED_H_ */
