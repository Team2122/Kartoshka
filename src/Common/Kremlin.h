/**
 * @file Kremlin.h
 * @author Lee Bousfield
 * @date Jan 19, 2015
 */

#include <CommandBase.h>
#include <map>

#ifndef KREMLIN_H_
#define KREMLIN_H_

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
	static CommandBase* Get(std::string fullName);
protected:
	/// Map of strings to commands
	static std::map<std::string, CommandBase*> commands;
	template <typename T>
	/**
	 * Creates the commands for a given class
	 * Loops the config searching for commands with the name of the class
	 */
	static void CreateCommandsForClass();
};

}

#endif
