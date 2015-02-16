/**
 * @file Kremlin.cpp
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#include <Commands/ClawForceHome.h>
#include <Commands/ClawPosition.h>
#include <Commands/Restack.h>
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
#include <Commands/StackTote.h>
#include "Commands/ArmShuttle.h"
#include "Commands/Cancel.h"
#include "Commands/AddTote.h"
#include "Commands/ResetTotes.h"
#include "Commands/ShuttleDown.h"
#include "Commands/HomeClaw.h"
#include "Commands/TopClaw.h"
#include "Commands/ClawRegripPosition.h"
#include "Commands/GroundToPlatform.h"
#include "Commands/RegripBin.h"
#include "Commands/ClawSmartRollers.h"
#include "Commands/PickBin.h"
#include "Commands/PlatformToPlacement.h"
#include "Commands/ClawToHome.h"
#include "Commands/ClawEstablishHome.h"

namespace tator {

std::map<std::string, Command*> Kremlin::commands;
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
			commands[totalName] = new T(totalName, node);
		}
	} else {
		commands[name] = new T(name, commandConfig);
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
	CreateCommandsForClass<Fingers>();
	CreateCommandsForClass<RecieveTote>();
	CreateCommandsForClass<UnloadTote>();
	CreateCommandsForClass<FlapFlappers>();
	CreateCommandsForClass<StackTote>();
	CreateCommandsForClass<ArmShuttle>();
	CreateCommandsForClass<Cancel>();
	CreateCommandsForClass<AddTote>();
	CreateCommandsForClass<ResetTotes>();
	CreateCommandsForClass<Restack>();
	CreateCommandsForClass<ShuttleDown>();
	CreateCommandsForClass<HomeClaw>();
	CreateCommandsForClass<TopClaw>();
	CreateCommandsForClass<ClawPosition>();
	CreateCommandsForClass<PickBin>();
	CreateCommandsForClass<RegripBin>();
	CreateCommandsForClass<GroundToPlatform>();
	CreateCommandsForClass<ClawRegripPosition>();
	CreateCommandsForClass<PlatformToPlacement>();
	CreateCommandsForClass<ClawEstablishHome>();
	CreateCommandsForClass<ClawToHome>();
	CreateCommandsForClass<ClawForceHome>();
}

Command* Kremlin::Get(std::string fullName) {
	if (commands.count(fullName) > 0) {
		return commands.at(fullName);
	} else {
		log.Error("Kremlin::Get(): no %s command", fullName.c_str());
		return nullptr;
	}
}

}
