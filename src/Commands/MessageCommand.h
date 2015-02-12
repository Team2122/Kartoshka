#include "CommandBase.h"
#include <yaml-cpp/yaml.h>

namespace tator {

class MessageCommand: public CommandBase {
public:
	MessageCommand(std::string name, YAML::Node params) :
			CommandBase(name) {
		message = params["message"].as<std::string>();
	}

	void Initialize() {
	}

	void Execute() {
		log.Info("%s", message.c_str());
	}

	bool IsFinished() {
		return true;
	}

	void End() {
	}

	void Interrupted() {
	}

	static std::string GetBaseName() {
		return "MessageCommand";
	}
protected:
	std::string message;
};

}
