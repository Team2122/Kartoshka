/**
 * @file RegripBin.h
 * @author Paul Vaughan
 * @date 2/15/2015
 */

#ifndef REGRIPBIN_H
#define REGRIPBIN_H

#include "CommandBase.h"
#include "Common/Kremlin.h"
#include "Commands/ClawClamp.h"
#include "Commands/ClawPosition.h"

namespace tator {

class RegripBin: public CommandGroup {
public:
	RegripBin(std::string name, YAML::Node config) {
		AddSequential(Kremlin::GetCopyOf<ClawClamp>("ClawClampContainer"));
		AddSequential(new WaitCommand(config["releaseWait"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<ClawPosition>("ClawPositionRegrip"));
		AddSequential(new WaitCommand(config["clampWait"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<ClawClamp>("ClawClampDeathGrip"));
	}

	static std::string GetBaseName() {
		return "RegripBin";
	}
};

}

#endif
