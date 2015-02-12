/**
 * @file ShuttleClamp.h
 * @author Alex Mikhalev
 * @date 2/9/2015
 */

#ifndef SHUTTLECLAMP_H_
#define SHUTTLECLAMP_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ShuttleClamp: public CommandBase {
public:
	ShuttleClamp(std::string name, YAML::Node config) :
			CommandBase(name) {
		open = name == "ShuttleClampOpen";
	}

	static std::string GetBaseName() {
		return "ShuttleClamp";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
	}

	virtual void Execute() {
		if (open) {
			shuttle->OpenProngs();
		} else {
			shuttle->CloseProngs();
		}
	}

	virtual bool IsFinished() {
		return true;
	}

	virtual void End() {
	}

	virtual void Interrupted() {
	}

protected:
	bool open;
};

}

#endif
