/**
 * @file Binder.h
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#include "CommandBase.h"

#ifndef BINDER_H_
#define BINDER_H_

namespace tator {

/**
 * Example trigger config
 * ---
 * 1: # Joystick Number
 *   1: # Button Number
 *     whileHeld: # Event Name
 *       - ExampleCommand # Command Name
 *       - OtherCommand
 *   2:
 *     whenPressed:
 *       - ExampleCommand
 */

/**
 * Binder class
 * Binds commands to joystick buttons
 */
class Binder {
public:
	/**
	 * Binds everything
	 */
	static void BindAll();
protected:
	/**
	 * Binds a command
	 */
	static void BindCommand(CommandBase* command, int joystick, int button,
			const std::string& event);
};

}

#endif
