/**
 * @file SubsystemBase.h
 * @author Lee Bousfield
 * @date Jan 17, 2015
 */

#ifndef SUBSYSTEMBASE_H_
#define SUBSYSTEMBASE_H_

#include <Commands/Subsystem.h>
#include "Logger.h"

namespace tator {

class Logger;

/**
 * The base class for all subsystems
 */
class SubsystemBase: public Subsystem {
public:
	SubsystemBase(const std::string& name);
	virtual ~SubsystemBase();

protected:
	Logger log;
};

} /* namespace tator */

#endif /* SUBSYSTEMBASE_H_ */
