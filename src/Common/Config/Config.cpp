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
	std::string configDir = std::string(kConfigDir);
	subsystems = YAML::LoadFile(configDir + "subsystems.yaml");
	commands = YAML::LoadFile(configDir + "commands.yaml");
	triggers = YAML::LoadFile(configDir + "triggers.yaml");
	YAML::Node subsysemOverrides = YAML::LoadFile(
			configDir + "subsystems-overrides.yaml");
	if (subsysemOverrides["override"]) {
		ApplyOverrides(subsystems, subsysemOverrides);
	}
}

void Config::ApplyOverrides(YAML::Node to, YAML::Node from) {
	if (to.IsMap() && from.IsMap()) {
		for (auto it = from.begin(); it != from.end(); ++it) {
			ApplyOverrides(to[it->first], it->second);
		}
	} else if ((to.IsSequence() && from.IsSequence())
			|| (to.IsScalar() && from.IsScalar())) {
		to = from;
	} else {
		throw std::runtime_error(
				"mismatch in override config: " + to.Scalar() + " vs "
						+ from.Scalar());
	}
}

void Config::Delete() {
	// We take puny little config and crush
	subsystems = YAML::Node();
	commands = YAML::Node();
	triggers = YAML::Node();
	// There no config here. There was never config.
}

}
