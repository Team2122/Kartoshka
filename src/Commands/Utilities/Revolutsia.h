/**
 * @file Revolutsia.h
 * @author Alex Mikhalev
 * @date Jul 15, 2015
 */

#ifndef REVOLUTSIA_H_
#define REVOLUTSIA_H_

#include "CommandBase.h"
#include "Common/Config/Kremlin.h"
#include <vector>

namespace tator {

/**
 * Cancels all of the possible dangerous commands that could possible be running.
 */
class Revolutsia: public CommandBase {
public:
	Revolutsia(std::string name, YAML::Node config) :
			CommandBase(name) {
		auto commandNames = config["commands"].as<std::vector<std::string>>();
		for (std::string name : commandNames) {
			commands.push_back(Kremlin::Get(name));
		}
	}

	static std::string GetBaseName() {
		return "Revolution";
	}

protected:
	void Execute() override {
		for (Command* command : commands) {
			if (command->IsRunning()) {
				command->Cancel();
			}
		}
	}

	bool IsFinished() override {
		return true;
	}

private:
	std::vector<Command*> commands;
};

}

#endif /* REVOLUTSIA_H_ */
