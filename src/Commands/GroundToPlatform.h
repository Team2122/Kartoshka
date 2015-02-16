/**
 * @file GroundToPlatform.h
 * @author Paul Vaughan
 * @date 2/15/2015
 */

#ifndef GROUNDTOPLATFORM_H
#define GROUNDTOPLATFORM_H

#include "CommandBase.h"
#include "Common/Kremlin.h"
#include "Commands/ClawPosition.h"
#include "Commands/ClawRotation.h"

namespace tator {

class GroundToPlatform: public CommandGroup {
public:
	GroundToPlatform(std::string name, YAML::Node config) {
		AddSequential(Kremlin::GetCopyOf<ClawPosition>("ClawPositionMiddle"));
		AddSequential(Kremlin::GetCopyOf<ClawRotation>("ClawRotationPlatform"));
		AddSequential(new WaitCommand(config["liftWait"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<ClawPosition>("ClawPositionPlatform"));
	}

	static std::string GetBaseName() {
		return "GroundToPlatform";
	}
};

}

#endif
