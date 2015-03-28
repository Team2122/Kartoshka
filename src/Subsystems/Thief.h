/**
 * @file Thief.h
 * @author Nick Hodes
 * @date March 28, 2015
 */

#ifndef THIEF_H
#define THIEF_H

#include <Solenoid.h>

#include "Common/SubsystemBase.h"
#include "Common/Config/Config.h"

namespace tator {
class Thief: public SubsystemBase {
public:
	enum class State {
		steal = 1, nosteal = 0
	};

	Thief(YAML::Node config);

	~Thief();

	void Set(State state);

private:
	Solenoid* piston;
};
}

#endif

