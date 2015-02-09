/**
 * @file Kremlin.cpp
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#include "Kremlin.h"
#include "Config.h"
#include <yaml-cpp/yaml.h>
#include <iostream>

#include "Commands/MessageCommand.h"
#include "Commands/DriveContinuous.h"
#include "Commands/HomeShuttle.h"
#include "Commands/ShuttlePosition.h"
#include "Commands/ShuttleClamp.h"

namespace tator {

std::map<std::string, Command*> Kremlin::commands;

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
			commands[totalName] = new T(node);
		}
	} else {
		commands[name] = new T(commandConfig);
	}
}

void Kremlin::CreateCommands() {
	CreateCommandsForClass<MessageCommand>();
	CreateCommandsForClass<DriveContinuous>();
	CreateCommandsForClass<HomeShuttle>();
	CreateCommandsForClass<ShuttlePosition>();
	CreateCommandsForClass<ShuttleClamp>();
}

Command* Kremlin::Get(std::string fullName) {
	return commands.at(fullName);
}

}
