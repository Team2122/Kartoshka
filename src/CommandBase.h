/**
 * @file CommandBase.h
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#ifndef COMMANDBASE_H_
#define COMMANDBASE_H_

#include <Commands/Command.h>
#include <yaml-cpp/yaml.h>
#include "Subsystems/Claw.h"
#include "Common/Logger.h"

namespace tator {

class Claw;
class Drive;
class Shuttle;
class Thief;
class ToteFeed;
class Otto;

/**
 * The base for all commands.
 */
class CommandBase: public Command {
	friend class Tester;
	friend class Robot;
	friend class Shuttle;
public:
	// Remeber to implement static std::string GetBaseName()
	CommandBase(const std::string& name);
	virtual ~CommandBase();
	/**
	 * Initializes all static subsystem pointers
	 */
	static void InitSubsystems(YAML::Node subsystem);

protected:
	Logger log;
	virtual void Initialize();
	virtual void Execute() = 0;
	virtual bool IsFinished() = 0;
	virtual void End();
	virtual void Interrupted();

	static Claw* claw;
	static Drive* drive;
	static Shuttle* shuttle;
	static Thief* thief;
	static ToteFeed* toteFeed;
	static Otto* otto;
};

/**
 * Defines the base methods needed for a command, including a constructor,
 * GetBaseName, and standard required command methods. The constructor, Execute,
 * and IsFinished must be implemented elsewhere.
 */
#define COMMAND_BASE_METHODS(command_name) \
	command_name(const std::string& name, YAML::Node config); \
	static std::string GetBaseName() { \
		return #command_name; \
	} \
protected: \
	void Execute() override; \
	bool IsFinished() override

/**
 * Defines everything defined in @c COMMAND_BASE_METHODS plus Initialize, End,
 * and Interrupted.
 */
#define COMMAND_METHODS(command_name) \
	COMMAND_BASE_METHODS(command_name); \
	void Initialize() override; \
	void End() override; \
	void Interrupted() override


/**
 * Defines the prototype for a constructor for a command. This exposes the name
 * of the command as a std::string reference named `name`, and the config for
 * the command as a YAML::Node named `config`.
 */
#define COMMAND_CONSTRUCTOR_DEF(command_name) \
	command_name::command_name(const std::string& name, YAML::Node config) \
		: CommandBase(name)


} /* namespace tator */

#endif /* COMMANDBASE_H_ */
