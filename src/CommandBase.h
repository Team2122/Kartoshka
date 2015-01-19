/**
 * @file CommandBase.h
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#ifndef COMMANDBASE_H_
#define COMMANDBASE_H_

#include <Commands/Command.h>
#include <yaml-cpp/yaml.h>

namespace tator {

class Claw;

/**
 * The base for all commands.
 */
class CommandBase: public Command {
public:
	// Remeber to implement static std::string GetBaseName()
	CommandBase();
	virtual ~CommandBase();
	/**
	* Initializes all static subsystem pointers
	*/
	static void InitSubsystems(YAML::Node subsystem);

protected:
	virtual void Initialize() = 0;
	virtual void Execute() = 0;
	virtual bool IsFinished() = 0;
	virtual void End() = 0;
	virtual void Interrupted() = 0;

	static Claw* claw;
};

} /* namespace tator */

#endif /* COMMANDBASE_H_ */
