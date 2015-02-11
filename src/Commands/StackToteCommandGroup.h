#ifndef STACKTOTECOMMANDGROUP_H
#define STACKTOTECOMMANDGROUP_H

#include "WPILib.h"
#include "CommandBase.h"
#include <yaml-cpp/yaml.h>

namespace tator {
class StackToteCommandGroup: public CommandGroup {
public:
	StackToteCommandGroup(std::string name, YAML::Node params) :
			CommandGroup(GetBaseName().c_str()) {
		AddSequential(Kremlin::Get("FingersHold"));
		AddSequential(Kremlin::Get("ShuttleClampOpen"));
		AddParallel(Kremlin::Get("ShuttlePositionDown"));
		AddSequential(new WaitCommand(params["waitTime"].as<double>()));
		AddSequential(Kremlin::Get("ShuttleClampClose"));
		AddSequential(Kremlin::Get("ShuttlePositionUp"));

	}

	static std::string GetBaseName() {
		return "StackToteCommandGroup";
	}

};
} /* namespace tator */

#endif /* STACKTOTECOMMANDGROUP_H */
