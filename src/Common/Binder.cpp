/**
 * @file Binder.cpp
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#include "Binder.h"
#include "Config.h"
#include "Kremlin.h"
#include <WPILib.h>
#include <iostream>

namespace tator {

void Binder::BindAll() {
	YAML::Node::iterator joyIt, buttonIt, eventIt, command;
	for (joyIt = Config::triggers.begin(); joyIt != Config::triggers.end();
			joyIt++) {
		for (buttonIt = joyIt->second.begin(); buttonIt != joyIt->second.end();
				buttonIt++) {
			for (eventIt = buttonIt->second.begin();
					eventIt != buttonIt->second.end(); eventIt++) {
				for (YAML::Node command : eventIt->second) {
					BindCommand(Kremlin::Get(command.as<std::string>()),
							joyIt->first.as<int>(), buttonIt->first.as<int>(),
							eventIt->first.as<std::string>());
				}
			}
		}
	}
}

void Binder::BindCommand(CommandBase* command, int joystick, int button,
		std::string event) {
	JoystickButton* buttonObj = new JoystickButton(
			Joystick::GetStickForPort(joystick), button);
	if (event == "WhenPressed") {
		buttonObj->WhenPressed(command);
	} else if (event == "WhileHeld") {
		buttonObj->WhileHeld(command);
	} else if (event == "WhenReleased") {
		buttonObj->WhenReleased(command);
	} else if (event == "CancelWhenPressed") {
		buttonObj->CancelWhenPressed(command);
	} else if (event == "ToggleWhenPressed") {
		buttonObj->ToggleWhenPressed(command);
	} else {
		throw "Invalid event " + event + " for command " + command->GetName();
	}
}

}
