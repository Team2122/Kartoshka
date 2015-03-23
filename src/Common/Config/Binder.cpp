/**
 * @file Binder.cpp
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#include "Binder.h"
#include <Joystick.h>
#include <Buttons/JoystickButton.h>
#include "Config.h"
#include "Kremlin.h"
#include <iostream>

namespace tator {

#define BindCommandWithVars BindCommand( \
		Kremlin::Get(bind.substr(bind.find(' ') + 1)), \
		joyIt->first.as<int>(), \
		buttonIt->first.as<int>(), \
		bind.substr(0, bind.find(' ')))

void Binder::BindAll() {
	YAML::Node::iterator joyIt, buttonIt;
	for (joyIt = Config::triggers.begin(); joyIt != Config::triggers.end();
			joyIt++) {
		for (buttonIt = joyIt->second.begin(); buttonIt != joyIt->second.end();
				buttonIt++) {
			if (buttonIt->second.IsSequence()) {
				for (YAML::Node bindNode : buttonIt->second) {
					std::string bind = bindNode.as<std::string>();
					BindCommandWithVars;
				}
			} else {
				std::string bind = buttonIt->second.as<std::string>();
				BindCommandWithVars;
			}
		}
	}
}

#undef BindCommandWithVars
// There no macro here
// There was never macro

void Binder::BindCommand(Command* command, int joystick, int button,
		const std::string& event) {
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
