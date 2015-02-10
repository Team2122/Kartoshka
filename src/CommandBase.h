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

/**
 * The base for all commands.
 */
class CommandBase: public Command {
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
};

} /* namespace tator */

#endif /* COMMANDBASE_H_ */
