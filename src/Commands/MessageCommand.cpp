#include <Commands/MessageCommand.h>
#include <iostream>

namespace tator {

MessageCommand::MessageCommand(YAML::Node params) {
	message = params["message"].as<std::string>();
	executed = false;
}

void MessageCommand::Initialize() {
}

void MessageCommand::Execute() {
	std::cerr << "MessageCommand message: " << message << std::endl;
	executed = true;
}

bool MessageCommand::IsFinished() {
	return executed;
}

void MessageCommand::End() {
}

void MessageCommand::Interrupted() {
}

std::string MessageCommand::GetBaseName() {
	return "MessageCommand";
}

}
