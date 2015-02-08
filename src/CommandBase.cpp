/**
 * @file CommandBase.cpp
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#include <CommandBase.h>
#include "Subsystems/Claw.h"
#include "Subsystems/Drive.h"
#include "Common/Logger.h"

namespace tator {

Claw* CommandBase::claw = nullptr;
Drive* CommandBase::drive = nullptr;

CommandBase::CommandBase(const std::string& name) :
		Command(name.c_str()), log(name.c_str()) {
}

CommandBase::~CommandBase() {
}

void CommandBase::Initialize() {
	 log.Info("%s has Initialized", GetName().c_str());
}

void CommandBase::End() {
	 log.Info("%s has Ended", GetName().c_str());
}

void CommandBase::Interrupted() {
	 log.Warn("%s has been Interrupted", GetName().c_str());
}

void CommandBase::InitSubsystems(YAML::Node subsystem) {
	claw = new Claw(subsystem["Claw"]);
	drive = new Drive(subsystem["Drive"]);
}

} /* namespace tator */
