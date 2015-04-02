/**
 * @file Kremlin.h
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#ifndef KREMLIN_H_
#define KREMLIN_H_

#include <CommandBase.h>
#include <map>
#include "Common/Logger.h"

namespace tator {

/**
 * Stores and creates commands
 */
class Kremlin {
public:
	/**
	 * Creates all the commands
	 */
	static void CreateCommands();
	/**
	 * Gets a command based on it's name
	 * @param fullName The full name of the command
	 * @return The command
	 */
	static Command* Get(std::string fullName);

	static Command* GetCopyOf(std::string fullName);
protected:
	/**
	 * Creates all the normal commands
	 */
	static void CreateNormalCommands();
	struct CommandDetails {
		Command* command;
		std::function<Command*()> constructor;
	};
	/// Map of strings to CommandDetails
	static std::map<std::string, CommandDetails> commands;
	static Logger log;
	static bool createdCommand; ///< If this iteration of command creation created a command
	static bool lastTry; ///< If this iteration of command creation is just to check for problems
	template<typename T>
	/**
	 * Creates the commands for a given class
	 * Loops the config searching for commands with the name of the class
	 */
	static void CreateCommandsForClass();
	/**
	 * Creates the command groups
	 * Special code is required to do dependency injection
	 */
	static void CreateGenericCommandGroups();
};

}

#endif
