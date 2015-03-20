/**
 * @file Otto.h
 * @author Nick Hodes
 * @date Jan 29, 2015
 */

#ifndef OTTO_H
#define OTTO_H

#include <WPILib.h>
#include "Common/SubsystemBase.h"
#include "Common/Config.h"

namespace tator {

class Otto: public SubsystemBase {
private:
	DigitalInput *autoSwitch0; ///< The first switch for auto
	DigitalInput *autoSwitch1; ///< The second switch for auto
	DigitalInput *autoSwitch2; ///< The third switch for auto
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

};

} /* namespace tator */

#endif /* OTTO_H */
