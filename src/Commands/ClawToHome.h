/**
 * @file ClawToHome.h
 * @author Paul Vaughan
 * @date 2/15/2015
 */

#ifndef CLAWTOHOME_H
#define CLAWTOHOME_H

#include "CommandBase.h"
#include "Common/Kremlin.h"
#include "Commands/ClawRotation.h"
#include "Commands/ClawPosition.h"
#include "Commands/HomeClaw.h"

namespace tator {

class ClawToHome : public CommandGroup {
public:
	ClawToHome(std::string name, YAML::Node config) {
		AddSequential(Kremlin::GetCopyOf<ClawRotation>("ClawRotationPick"));
		AddSequential(new WaitCommand(config["rotateWait"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<HomeClaw>("HomeClaw"));
	}

	static std::string GetBaseName() {
		return "ClawToHome";
	}
};

}

#endif
