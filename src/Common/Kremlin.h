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
	static Command* Get(std::string fullName);

	template<typename T>
	static Command* GetCopyOf(std::string fullName);
protected:
	/// Map of strings to commands
	static std::map<std::string, Command*> commands;
	template<typename T>
	/**
	 * Creates the commands for a given class
	 * Loops the config searching for commands with the name of the class
	 */
	static void CreateCommandsForClass();
};

template<typename T>
Command* Kremlin::GetCopyOf(std::string fullName) {
	std::string name = T::GetBaseName();
	YAML::Node config = Config::commands;
	if (config[name].IsMap()) {
		return new T(fullName, config[name]);
	} else if (config[name].IsSequence()) {
		std::string extension = fullName.substr(name.length());
		for (YAML::Node part : config[name]) {
			if (part["name"].as<std::string>() == extension)
				return new T(fullName, part);
		}
	}
	throw std::runtime_error("No such command: " + fullName);
}

}

#endif
