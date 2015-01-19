/**
 * @file CommandBase.cpp
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#include <CommandBase.h>
#include "Subsystems/Claw.h"

namespace tator {

Claw* CommandBase::claw = nullptr;

CommandBase::CommandBase() {

}

CommandBase::~CommandBase() {
}

void CommandBase::InitSubsystems(YAML::Node subsystem) {
	claw = new Claw(subsystem["Claw"]);
}

} /* namespace tator */
