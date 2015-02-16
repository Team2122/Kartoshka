/**
 * @file Kremlin.cpp
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#include <Commands/ClawForceHome.h>
#include <Commands/ClawPosition.h>
#include "Kremlin.h"
#include "Config.h"
#include <yaml-cpp/yaml.h>
#include <iostream>

#include "Commands/MessageCommand.h"
#include "Commands/DriveContinuous.h"
#include "Commands/HomeShuttle.h"
#include "Commands/ShuttlePosition.h"
#include "Commands/ShuttleClamp.h"
#include "Commands/ClawRotation.h"
#include "Commands/ClawRollers.h"
#include "Commands/ClawClamp.h"
#include "Commands/Fingers.h"
#include "Commands/RecieveTote.h"
#include "Commands/UnloadTote.h"
#include "Commands/FlapFlappers.h"
#include "Commands/ArmShuttle.h"
#include "Commands/Cancel.h"
#include "Commands/AddTote.h"
#include "Commands/ResetTotes.h"
#include "Commands/ShuttleDown.h"
#include "Commands/HomeClaw.h"
#include "Commands/TopClaw.h"
#include "Commands/ClawRegripPosition.h"
#include "Commands/ClawSmartRollers.h"
#include "Commands/ClawEstablishHome.h"
#include "Commands/OttoTestCommandGroup.h"
#include "Commands/GenericCommandGroup.h"
#include "Commands/ClawRotationContinuous.h"

namespace tator {

std::map<std::string, Kremlin::CommandDetails> Kremlin::commands;
Logger Kremlin::log("Kremlin");

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
					[totalName, node] () {return new T(totalName, node);} };
			commands[totalName] = details;
		}
	} else {
		CommandDetails details = { new T(name, commandConfig),
				[name, commandConfig] () {return new T(name, commandConfig);} };
		commands[name] = details;
	}
}

void Kremlin::CreateCommands() {
	CreateCommandsForClass<MessageCommand>();
	CreateCommandsForClass<DriveContinuous>();
	CreateCommandsForClass<HomeShuttle>();
	CreateCommandsForClass<ShuttlePosition>();
	CreateCommandsForClass<ShuttleClamp>();
	CreateCommandsForClass<ClawClamp>();
	CreateCommandsForClass<ClawRotation>();
	CreateCommandsForClass<ClawRollers>();
	CreateCommandsForClass<ClawSmartRollers>();
	CreateCommandsForClass<Fingers>();
	CreateCommandsForClass<RecieveTote>();
	CreateCommandsForClass<UnloadTote>();
	CreateCommandsForClass<FlapFlappers>();
	CreateCommandsForClass<AddTote>();
	CreateCommandsForClass<ResetTotes>();
	CreateCommandsForClass<ShuttleDown>();
	CreateCommandsForClass<HomeClaw>();
	CreateCommandsForClass<TopClaw>();
	CreateCommandsForClass<ClawPosition>();
	CreateCommandsForClass<ClawRegripPosition>();
	CreateCommandsForClass<ClawEstablishHome>();
	CreateCommandsForClass<ClawForceHome>();
	CreateCommandsForClass<OttoTestCommandGroup>();
	CreateCommandsForClass<GenericCommandGroup>();
	CreateCommandsForClass<ArmShuttle>();
	CreateCommandsForClass<Cancel>();
	CreateCommandsForClass<ClawRotationContinuous>();
}

Command* Kremlin::Get(std::string fullName) {
	if (commands.count(fullName) > 0) {
		return commands.at(fullName).command;
	} else {
		log.Error("Kremlin::Get(): no %s command", fullName.c_str());
		return nullptr;
	}
}

Command* Kremlin::GetCopyOf(std::string fullName) {
	if (commands.count(fullName) > 0) {
		return commands.at(fullName).constructor();
	} else {
		log.Error("Kremlin::GetCopyOf(): no %s command", fullName.c_str());
		return nullptr;
	}
}

}
