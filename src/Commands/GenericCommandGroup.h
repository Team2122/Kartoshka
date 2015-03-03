/**
 * @file GenericCommandGroup.h
 * @date Feb 14, 2015
 * @author Lee Bousfield
 */

#ifndef GENERICCOMMANDGROUP_H_
#define GENERICCOMMANDGROUP_H_

#include "CommandBase.h"

namespace tator {
class GenericCommandGroup: public CommandGroup {
public:
	GenericCommandGroup(std::string name, YAML::Node config) {
		for (YAML::Node command : config["commands"]) {
			if (command.IsMap()) {
				std::string name = command["name"].as<std::string>();
				if (name == "WaitCommand") {
					AddSequential(
							new WaitCommand(command["time"].as<double>()));
				} else if (command["execution"].as<std::string>()
						== "Parallel") {
					AddParallel(Kremlin::GetCopyOf(name));
				} else {
					AddSequential(Kremlin::GetCopyOf(name));
				}
			} else {
				AddSequential(Kremlin::GetCopyOf(command.as<std::string>()));
			}
		}
	}

	static std::string GetBaseName() {
		return "$";
	}
};
}

#endif /* GENERICCOMMANDGROUP_H_ */
