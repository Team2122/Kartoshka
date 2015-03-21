/**
 * @file Kremlin.cpp
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#include "Kremlin.h"
#include "Config.h"
#include <yaml-cpp/yaml.h>
#include <iostream>

#include "Commands/BumpBottomTote.h"
#include "Commands/Cancel.h"
#include "Commands/ClawClamp.h"
#include "Commands/ClawClampToggle.h"
#include "Commands/ClawEstablishHome.h"
#include "Commands/ClawForceHome.h"
#include "Commands/ClawPosition.h"
#include "Commands/ClawRamp.h"
#include "Commands/ClawRollers.h"
#include "Commands/ClawRotation.h"
#include "Commands/ClawRotationContinuous.h"
#include "Commands/ClawSmartRollers.h"
#include "Commands/DriveContinuous.h"
#include "Commands/DriveDistance.h"
#include "Commands/Fingers.h"
#include "Commands/FlapFlappers.h"
#include "Commands/GenericCommandGroup.h"
#include "Commands/HoldBottomTote.h"
#include "Commands/HomeClaw.h"
#include "Commands/HomeShuttle.h"
#include "Commands/IntakeTotes.h"
#include "Commands/LackOfStack.h"
#include "Commands/ManualClawLift.h"
#include "Commands/MessageCommand.h"
#include "Commands/RecieveBottomTote.h"
#include "Commands/RollerbedPiston.h"
#include "Commands/RollerbedPistonToggle.h"
#include "Commands/SetDesiredTotes.h"
#include "Commands/ShuttleClamp.h"
#include "Commands/ShuttleDown.h"
#include "Commands/ShuttleHold.h"
#include "Commands/ShuttlePosition.h"
#include "Commands/ShuttleRamp.h"
#include "Commands/TopClaw.h"
#include "Commands/ToteDirector.h"
#include "Commands/UnloadTote.h"
#include "Commands/UpdateTotesRatcheted.h"

namespace tator {

std::map<std::string, Kremlin::CommandDetails> Kremlin::commands;
Logger Kremlin::log("Kremlin");

void Kremlin::CreateCommands() {
	CreateCommandsForClass<MessageCommand>();
	CreateCommandsForClass<DriveContinuous>();
	CreateCommandsForClass<HomeShuttle>();
	CreateCommandsForClass<ShuttlePosition>();
	CreateCommandsForClass<ShuttleRamp>();
	CreateCommandsForClass<ShuttleClamp>();
	CreateCommandsForClass<ClawClamp>();
	CreateCommandsForClass<ClawRotation>();
	CreateCommandsForClass<ClawRollers>();
	CreateCommandsForClass<ClawSmartRollers>();
	CreateCommandsForClass<Fingers>();
	CreateCommandsForClass<RecieveBottomTote>();
	CreateCommandsForClass<UnloadTote>();
	CreateCommandsForClass<FlapFlappers>();
	CreateCommandsForClass<ShuttleDown>();
	CreateCommandsForClass<HomeClaw>();
	CreateCommandsForClass<TopClaw>();
	CreateCommandsForClass<ClawPosition>();
	CreateCommandsForClass<ClawEstablishHome>();
	CreateCommandsForClass<ClawRotationContinuous>();
	CreateCommandsForClass<ClawForceHome>();
	CreateCommandsForClass<ManualClawLift>();
	CreateCommandsForClass<DriveDistance>();
	CreateCommandsForClass<LackOfStack>();
	CreateCommandsForClass<ClawClampToggle>();
	CreateCommandsForClass<RollerbedPiston>();
	CreateCommandsForClass<RollerbedPistonToggle>();
	CreateCommandsForClass<ClawRamp>();
	CreateCommandsForClass<BumpBottomTote>();
	CreateCommandsForClass<HoldBottomTote>();
	CreateCommandsForClass<UpdateTotesRatcheted>();
	CreateCommandsForClass<SetDesiredTotes>();
	CreateCommandsForClass<IntakeTotes>();
	CreateCommandsForClass<ShuttleHold>();
	CreateCommandsForClass<GenericCommandGroup>();
	CreateCommandsForClass<ToteDirector>();
	CreateCommandsForClass<Cancel>();
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
					[totalName, node] () {return new T(totalName, node);} };
			commands[totalName] = details;
		}
	} else {
		CommandDetails details = { new T(name, commandConfig),
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
