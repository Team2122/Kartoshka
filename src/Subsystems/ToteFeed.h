/**
 * @file ToteFeed.h
 * @author Lee Bousfield
 * @date Jan 24, 2015
 */

#ifndef TOTEFEED_H_
#define TOTEFEED_H_

#include <yaml-cpp/yaml.h>
#include <Talon.h>
#include "Common/SubsystemBase.h"
#include "Common/Hardware/FixedField.h"

namespace tator {

/**
 * For the ToteFeed/Conveyor
 * Moves totes
 */
class ToteFeed: public SubsystemBase {
	friend class Tester;
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
	 * Sets the flappers
	 * @param flappers the speed of the flappers
	 */
	void SetFlapperSpeed(float speed);

private:
	Talon* rollers; ///< The roller motors
	Talon* flappers; ///< The flapper motors
	FixedField* backSensor; ///< The back sensor
};

}

#endif /* TOTEFEED_H_ */
