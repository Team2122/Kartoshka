/**
 * @file RegripBin.h
 * @author Paul Vaughan
 * @date 2/15/2015
 */

#ifndef PICKBIN_H
#define PICKBIN_H

#include "CommandBase.h"
#include "Common/Kremlin.h"
#include "Commands/ClawClamp.h"
#include "Commands/ClawSmartRollers.h"


namespace tator {

class PickBin: public CommandGroup {
public:
	PickBin(std::string name, YAML::Node config) {
		AddSequential(Kremlin::GetCopyOf<ClawClamp>("ClawClampContainer"));
		AddSequential(Kremlin::GetCopyOf<ClawSmartRollers>("ClawSmartRollersIn"));
		AddSequential(new WaitCommand(config["smartWait"].as<double>()));
		AddSequential(Kremlin::GetCopyOf<ClawClamp>("ClawClampDeathGrip"));
	}

	static std::string GetBaseName() {
		return "PickBin";
	}
};

}

#endif
