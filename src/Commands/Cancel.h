/**
 * @file Cancel.h
 * @author Daniele Moro
 * @date 2/13/2015
 */

#ifndef CANCEL_H_
#define CANCEL_H_

#include "CommandBase.h"
#include "Common/Kremlin.h"

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

protected:
	void Execute() override {
		if (command->IsRunning()) {
			command->Cancel();
		}
	}

	bool IsFinished() override {
		return true;
	}

private:
	Command* command;
};

}

#endif
