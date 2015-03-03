/**
 * @file Cancel.h
 * @author Daniele Moro
 * @date 2/13/2015
 */

#ifndef CANCEL_H_
#define CANCEL_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class Cancel: public CommandBase {
public:
	Cancel(std::string name, YAML::Node config) :
			CommandBase(name) {
		std::string commandName = config["name"].as<std::string>();
		command = Kremlin::Get(commandName);
	}

	static std::string GetBaseName() {
		return "Cancel";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
	}

	virtual void Execute() {
		if (command->IsRunning()) {
			command->Cancel();
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
	Command* command;
};

}

#endif
