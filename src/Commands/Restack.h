/**
 * @file Restack.h
 * @author Alex Mikhalev
 * @date 2/14/2015
 */

#ifndef RESTACK_H_
#define RESTACK_H_

#include "CommandBase.h"
#include "Common/Kremlin.h"
#include "ShuttleDown.h"
#include "ShuttlePosition.h"
#include "ShuttleClamp.h"
#include "Fingers.h"
#include "UnloadTote.h"
#include "ResetTotes.h"

namespace tator {

class Restack : public CommandGroup {
public:
	Restack(std::string name, YAML::Node config) {
		AddSequential(Kremlin::GetCopyOf<ShuttleClamp>("ShuttleClampOpen"));
		AddSequential(Kremlin::GetCopyOf<ShuttlePosition>("ShuttlePositionDown"));
		AddSequential(Kremlin::GetCopyOf<ShuttleClamp>("ShuttleClampClose"));
		AddSequential(new WaitCommand(config["lowerWait"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<ShuttlePosition>("ShuttlePositionUnload"));
		AddSequential(Kremlin::GetCopyOf<Fingers>("FingersRelease"));
		AddSequential(new WaitCommand(config["fingerWait"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<ShuttleDown>("ShuttleDown"));
		AddSequential(Kremlin::GetCopyOf<ShuttlePosition>("ShuttlePositionDown"));
		AddSequential(Kremlin::GetCopyOf<ShuttleClamp>("ShuttleClampOpen"));
		AddSequential(Kremlin::GetCopyOf<ResetTotes>("ResetTotes"));
	}

	static std::string GetBaseName() {
		return "Restack";
	}
};

}

#endif
