/**
 * @file Config.cpp
 * @author Lee Bousfield
 * @date Jan 18, 2015
 */

#include "Config.h"
#include <yaml-cpp/yaml.h>

namespace tator {

YAML::Node Config::subsystems;
YAML::Node Config::commands;
YAML::Node Config::triggers;

void Config::Load() {
	subsystems = YAML::LoadFile(std::string(kConfigDir) + "subsystems.yaml");
	commands = YAML::LoadFile(std::string(kConfigDir) + "commands.yaml");
	triggers = YAML::LoadFile(std::string(kConfigDir) + "triggers.yaml");
}

void Config::Delete() {
	// We take puny little config and crush
	subsystems = YAML::Node();
	commands = YAML::Node();
	triggers = YAML::Node();
	// There no config here. There was never config.
}

}
