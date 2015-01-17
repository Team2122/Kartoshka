/**
 * @file SubsystemBase.cpp
 * @author Lee Bousfield
 * @date Jan 17, 2015
 */

#include "SubsystemBase.h"

namespace tator {

SubsystemBase::SubsystemBase(const std::string& name) :
		Subsystem(name.c_str()) {

}

SubsystemBase::~SubsystemBase() {

}

} /* namespace tator */
