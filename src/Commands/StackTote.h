#ifndef STACKTOTE_H
#define STACKTOTE_H

#include "WPILib.h"
#include "CommandBase.h"
#include <yaml-cpp/yaml.h>

#include "Fingers.h"
#include "ShuttleClamp.h"
#include "ShuttlePosition.h"
#include "AddTote.h"

namespace tator {
class StackTote: public CommandGroup {
public:
	StackTote(std::string name, YAML::Node params) :
			CommandGroup(GetBaseName().c_str()) {
		AddSequential(new WaitCommand(params["sensorWait"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<Fingers>("FingersHold"));
		AddSequential(Kremlin::GetCopyOf<ShuttleClamp>("ShuttleClampOpen"));
		AddSequential(Kremlin::GetCopyOf<ShuttlePosition>("ShuttlePositionDown"));
		AddSequential(Kremlin::GetCopyOf<ShuttleClamp>("ShuttleClampClose"));
		AddSequential(new WaitCommand(params["waitTime"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<ShuttlePosition>("ShuttlePositionUp"));
		AddSequential(new WaitCommand(params["waitTime"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<ShuttleClamp>("ShuttleClampOpen"));
		AddSequential(Kremlin::GetCopyOf<ShuttlePosition>("ShuttlePositionDown"));
		AddSequential(Kremlin::GetCopyOf<AddTote>("AddTote"));
	}

	static std::string GetBaseName() {
		return "StackTote";
	}

};
} /* namespace tator */

#endif /* STACKTOTE_H */
