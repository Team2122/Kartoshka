#ifndef OTTOTESTCOMMANDGROUP_H
#define OTTOTESTCOMMANDGROUP_H

#include "WPILib.h"
#include "CommandBase.h"
#include <yaml-cpp/yaml.h>

namespace tator {
class OttoTestCommandGroup: public CommandGroup {
public:
	OttoTestCommandGroup(std::string name, YAML::Node params) :
			CommandGroup(name.c_str()) {
		AddSequential(new WaitCommand(5.0));
		AddSequential(Kremlin::Get("MessageCommand"));
	}

	static std::string GetBaseName() {
		return "OttoTestCommandGroup";
	}
};
} /* namespace tator */

#endif /* OTTOTESTCOMMAND_H */
