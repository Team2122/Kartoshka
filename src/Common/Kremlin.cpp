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

namespace tator {

std::map<std::string, CommandBase*> Kremlin::commands;

template<typename T>
void Kremlin::CreateCommandsForClass() {
	std::string name = T::GetBaseName();
	YAML::Node config = Config::commands;
	YAML::Node::iterator configIt, nodeIt;
	for (configIt = config.begin(); configIt != config.end(); ++configIt) {
		if (configIt->first.as<std::string>() == name) {
			for (YAML::Node node : configIt->second) {
				std::string totalName;
				std::cerr << node["name"].as<std::string>() << std::endl;
				if (node["name"].IsNull()) {
					totalName = name;
				} else {
					totalName = name + node["name"].as<std::string>();
				}
				commands[totalName] = new T(node);
			}
		}
	}
}

void Kremlin::CreateCommands() {
	CreateCommandsForClass<MessageCommand>();
}

CommandBase* Kremlin::Get(std::string fullName) {
	return commands.at(fullName);
}

}
