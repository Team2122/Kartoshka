#ifndef CLAWGRABCOMMANDGROUP_H
#define CLAWGRABCOMMANDGROUP_H

#include "WPILib.h"
#include "CommandBase.h"
#include <yaml-cpp/yaml.h>

namespace tator {
class ClawGrabCommandGroup: public CommandGroup {
public:
	ClawGrabCommandGroup(std::string name, YAML::Node params) :
		CommandGroup(GetBaseName().c_str()) {
		AddSequential(Kremlin::Get("ClawRotationPick"));
		AddSequential(Kremlin::Get("ShuttlePositionDown")); //? I don't know if this is the correct command
		AddParallel(Kremlin::Get("ClawClampReleased"));
		AddParallel(Kremlin::Get("ClawRollersIn"));
		AddSequential(new WaitCommand(2.0)); // For now until we can get a "Check For Sensor"
		AddSequential(Kremlin::Get("ClawRollersOff"));
		AddParallel(Kremlin::Get("ClawClampContainer"));
		AddSequential(Kremlin::Get("ShuttlePositionUp")); // Same as before
		AddSequential(Kremlin::Get("ClawRotationPlatform"));
	}

	static std::string GetBaseName() {
		return "ClawGrabCommandGroup";
	}
};
} /* namespace tator */

#endif /* CLAWGRABCOMMANDGROUP_H */
