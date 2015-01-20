#include "CommandBase.h"
#include <yaml-cpp/yaml.h>

namespace tator {

class MessageCommand: public CommandBase {
public:
	MessageCommand(YAML::Node params);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
	static std::string GetBaseName();
protected:
	std::string message;
	bool executed;
};

}
