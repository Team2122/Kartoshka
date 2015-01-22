#include "CommandBase.h"
#include <yaml-cpp/yaml.h>

namespace tator {

class MessageCommand: public CommandBase {
public:
	MessageCommand(YAML::Node params) {
		message = params["message"].as<std::string>();
	}

	void Initialize() {
	}

	void Execute() {
		std::cerr << "MessageCommand message: " << message << std::endl;
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
