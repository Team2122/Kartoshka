/**
 * @file GenericCommandGroup.h
 * @date Feb 14, 2015
 * @author Lee Bousfield
 */

#ifndef GENERICCOMMANDGROUP_H_
#define GENERICCOMMANDGROUP_H_

#include "CommandBase.h"
#include <Commands/CommandGroup.h>
#include <Commands/WaitCommand.h>
#include "Common/Config/Kremlin.h"

namespace tator {
class GenericCommandGroup: public CommandGroup {
public:
	GenericCommandGroup(std::string name, YAML::Node config) {
		for (YAML::Node command : config) {
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
		return "GenericCommandGroup";
	}
};

}

#endif /* GENERICCOMMANDGROUP_H_ */
