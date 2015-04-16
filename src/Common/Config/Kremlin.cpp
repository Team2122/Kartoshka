/**
 * @file Kremlin.cpp
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#include "Kremlin.h"
#include "Config.h"
#include <yaml-cpp/yaml.h>
#include <iostream>

#include "Commands/Claw/ClawClamp.h"
#include "Commands/Claw/ClawClampToggle.h"
#include "Commands/Claw/ClawEstablishHome.h"
#include "Commands/Claw/ClawRightBin.h"
#include "Commands/Claw/ClawSmartRollers.h"
#include "Commands/Claw/ClawToAngle.h"
#include "Commands/Claw/ClawToHeight.h"
#include "Commands/Claw/ManualClawLift.h"

#include "Commands/Drive/DriveArc.h"
#include "Commands/Drive/DriveContinuous.h"
#include "Commands/Drive/DriveDistance.h"
#include "Commands/Drive/DriveRotate.h"
#include "Commands/Drive/DriveStraight.h"
#include "Commands/Drive/ResetAngle.h"

#include "Commands/Shuttle/Fingers.h"
#include "Commands/Shuttle/HomeShuttle.h"
#include "Commands/Shuttle/ShuttleClamp.h"
#include "Commands/Shuttle/ShuttleDown.h"
#include "Commands/Shuttle/ShuttleHold.h"
#include "Commands/Shuttle/ShuttlePosition.h"
#include "Commands/Shuttle/ShuttleRamp.h"

#include "Commands/Thief/ThiefGrabber.h"
#include "Commands/Thief/ThiefToggle.h"

#include "Commands/ToteDictator/SetDesiredTotes.h"
#include "Commands/ToteDictator/ToteDictator.h"
#include "Commands/ToteDictator/UpdateTotesRatcheted.h"

#include "Commands/ToteFeed/BumpBottomTote.h"
#include "Commands/ToteFeed/HoldBottomTote.h"
#include "Commands/ToteFeed/IntakeTotes.h"
#include "Commands/ToteFeed/LackOfStack.h"
#include "Commands/ToteFeed/RecieveBottomTote.h"
#include "Commands/ToteFeed/UnloadTote.h"

#include "Commands/Utilities/Cancel.h"
#include "Commands/Utilities/GenericCommandGroup.h"

namespace tator {

std::map<std::string, Kremlin::CommandDetails> Kremlin::commands;
Logger Kremlin::log("Kremlin");

void Kremlin::CreateNormalCommands() {
	// Claw
	CreateCommandsForClass<ClawClamp>();
	CreateCommandsForClass<ClawClampToggle>();
	CreateCommandsForClass<ClawEstablishHome>();
	CreateCommandsForClass<ClawRightBin>();
	CreateCommandsForClass<ClawSmartRollers>();
	CreateCommandsForClass<ClawToAngle>();
	CreateCommandsForClass<ClawToHeight>();
	CreateCommandsForClass<ManualClawLift>();

	// Drive
	CreateCommandsForClass<DriveArc>();
	CreateCommandsForClass<DriveContinuous>();
	CreateCommandsForClass<DriveDistance>();
	CreateCommandsForClass<DriveRotate>();
	CreateCommandsForClass<DriveStraight>();
	CreateCommandsForClass<ResetAngle>();

	// Shuttle
	CreateCommandsForClass<Fingers>();
	CreateCommandsForClass<HomeShuttle>();
	CreateCommandsForClass<ShuttlePosition>();
	CreateCommandsForClass<ShuttleRamp>();
	CreateCommandsForClass<ShuttleClamp>();
	CreateCommandsForClass<ShuttleDown>();
	CreateCommandsForClass<ShuttleHold>();

	// Thief
	CreateCommandsForClass<ThiefGrabber>();
	CreateCommandsForClass<ThiefToggle>();

	// ToteFeed
	CreateCommandsForClass<BumpBottomTote>();
	CreateCommandsForClass<HoldBottomTote>();
	CreateCommandsForClass<IntakeTotes>();
	CreateCommandsForClass<LackOfStack>();
	CreateCommandsForClass<RecieveBottomTote>();
	CreateCommandsForClass<UnloadTote>();

	// Utilities
	CreateCommandsForClass<Cancel>();

	// ToteDictator
	CreateCommandsForClass<SetDesiredTotes>();
	CreateCommandsForClass<UpdateTotesRatcheted>();
	CreateCommandsForClass<ToteDictator>();
}

void Kremlin::CreateCommands() {
	Kremlin::CreateNormalCommands();
	bool lastTry = false;
	bool createdCommand = false;
	do {
		createdCommand = Kremlin::CreateGenericCommandGroups(lastTry);
		if (!createdCommand) {
			lastTry = !lastTry;
		}
	} while (createdCommand || lastTry);
}

bool Kremlin::CreateGenericCommandGroups(bool lastTry) {
	bool createdCommand = false;
	// Special GenericCommandGroup code
	for (YAML::Node::iterator it = Config::commands.begin();
			it != Config::commands.end(); it++) {
		std::string fullName = it->first.as<std::string>();
		if (fullName.at(0) == '$') {
			YAML::Node commandConfig = it->second;
			bool missingCommand = false;
			if (commands.count(fullName) > 0) {
				// We already created the command
				continue;
			}
			for (YAML::Node command : commandConfig) {
				std::string dependency;
				if (command.IsMap()) {
					dependency = command["name"].as<std::string>();
					if (dependency == "WaitCommand") {
						continue;
					}
				} else {
					dependency = command.as<std::string>();
				}
				if (commands.count(dependency) == 0) {
					if (lastTry) {
						log.Error(
								"CommandGroup %s could not be created as %s is missing.",
								fullName.c_str(), dependency.c_str());
					}
					missingCommand = true;
					break;
				}
			}
			if (missingCommand) {
				continue;
			}
			CommandDetails details = { new GenericCommandGroup(fullName, commandConfig),
					[fullName, commandConfig] () {
						return new GenericCommandGroup(fullName, commandConfig);
					} };
			commands[fullName] = details;
			createdCommand = true;
		}
	}
	return createdCommand;
}

template<typename T>
void Kremlin::CreateCommandsForClass() {
	std::string name = T::GetBaseName();
	YAML::Node commandConfig = Config::commands[name];
	if (!commandConfig.IsDefined()) {
		return;
	}
	if (commandConfig.IsSequence()) {
		for (YAML::Node node : commandConfig) {
			std::string totalName = name;
			if (node["name"].IsScalar()) {
				totalName += node["name"].as<std::string>();
			}
			// This is a lambda
			// See http://www.cprogramming.com/c++11/c++11-lambda-closures.html
			CommandDetails details = { new T(totalName, node),
					[totalName, node] () {
						return new T(totalName, node);
					} };
			commands[totalName] = details;
		}
	} else {
		CommandDetails details =
				{ new T(name, commandConfig),
						[name, commandConfig] () {return new T(name, commandConfig);} };
		commands[name] = details;
	}
}

Command* Kremlin::Get(std::string fullName) {
	if (commands.count(fullName) > 0) {
		return commands.at(fullName).command;
	} else {
		log.Error("Kremlin::Get(): no %s command", fullName.c_str());
		throw std::runtime_error(
				"Kremlin::Get(): No such command: " + fullName);
		return nullptr;
	}
}

Command* Kremlin::GetCopyOf(std::string fullName) {
	if (commands.count(fullName) > 0) {
		return commands.at(fullName).constructor();
	} else {
		log.Error("Kremlin::GetCopyOf(): no %s command", fullName.c_str());
		throw std::runtime_error(
				"Kremlin::GetCopyOf(): No such command: " + fullName);
		return nullptr;
	}
}

}
