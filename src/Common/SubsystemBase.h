/**
 * @file SubsystemBase.h
 * @author Lee Bousfield
 * @date Jan 17, 2015
 */

#ifndef SUBSYSTEMBASE_H_
#define SUBSYSTEMBASE_H_

#include <WPILib.h>

namespace tator {

/**
 * The base class for all subsystems
 */
class SubsystemBase: public Subsystem {
public:
	SubsystemBase(const std::string& name);
	virtual ~SubsystemBase();
};

} /* namespace tator */

#endif /* SUBSYSTEMBASE_H_ */
