/**
 * @file PlatformToPlacement.h
 * @author Paul Vaughan
 * @date 2/15/2015
 */

#ifndef PLATFORMTOPLACEMENT_H
#define PLATFORMTOPLACEMENT_H

#include "CommandBase.h"
#include "Common/Kremlin.h"
#include "Commands/ClawPosition.h"
#include "Commands/ClawRotation.h"
#include "Commands/ClawClamp.h"

namespace tator {

class PlatformToPlacement: public CommandGroup {
public:
	PlatformToPlacement(std::string name, YAML::Node config) {
		AddSequential(Kremlin::GetCopyOf<TopClaw>("ClawPositionTop"));
		AddSequential(Kremlin::GetCopyOf<ClawRotation>("ClawRotationPick"));
	}

	static std::string GetBaseName() {
		return "PlatformToPlacement";
	}
};

}

#endif
