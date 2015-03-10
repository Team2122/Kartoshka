#ifndef MESSAGECOMMAND_H_
#define MESSAGECOMMAND_H_

#include "CommandBase.h"

namespace tator {

class MessageCommand: public CommandBase {
public:
	MessageCommand(std::string name, YAML::Node params) :
			CommandBase(name) {
		message = params["message"].as<std::string>();
	}

	static std::string GetBaseName() {
		return "MessageCommand";
	}

protected:
	void Execute() override {
		log.Info("%s", message.c_str());
	}

	bool IsFinished() override {
		return true;
	}

private:
	std::string message;
};

}

#endif /* MESSAGECOMMAND_H_ */
