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
	DigitalInput *autoSwitch1;
	DigitalInput *autoSwitch2;
	DigitalInput *autoSwitch3;
public:
	/**
	 * The constructor for Otto
	 */
	Otto(YAML::Node config);

	/**
	 * The destructor for Otto
	 */
	~Otto();

	int GetAutoModeNumber();

};

} /* namespace tator */

#endif /* OTTO_H */
