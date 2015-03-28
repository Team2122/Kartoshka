/**
 * @file CommandBase.cpp
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#include <CommandBase.h>
#include "Subsystems/Claw.h"
#include "Subsystems/Drive.h"
#include "Subsystems/Claw.h"
#include "Subsystems/Otto.h"
#include "Common/Logger.h"
#include "Subsystems/Shuttle.h"
#include "Subsystems/Thief.h"
#include "Subsystems/ToteFeed.h"

namespace tator {

Claw* CommandBase::claw = nullptr;
Drive* CommandBase::drive = nullptr;
Shuttle* CommandBase::shuttle = nullptr;
Thief* CommandBase::thief = nullptr;
ToteFeed* CommandBase::toteFeed = nullptr;
Otto* CommandBase::otto = nullptr;

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
	shuttle = new Shuttle(subsystem["Shuttle"]);
	thief = new Thief(subsystem["Thief"]);
	toteFeed = new ToteFeed(subsystem["ToteFeed"]);
	otto = new Otto(subsystem["Otto"]);
}

} /* namespace tator */
