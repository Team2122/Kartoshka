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
		up, down
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
	 * @param flappers if the flappers should be on or off
	 */
	void SetFlappers(bool set);

private:
	/// Rollers
	Talon* rollers;
	/// Flappers
	Talon* flappers;
	/// Back beam sensor
	DigitalInput* backSensor;
	/// Piston to move rollers up
	Solenoid* rollerPiston;
	/// The speed to run the flappers at
	double flapperSpeed;
};

}

#endif /* TOTEFEED_H_ */
