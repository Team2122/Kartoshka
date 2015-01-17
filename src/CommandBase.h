/**
 * @file CommandBase.h
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#ifndef COMMANDBASE_H_
#define COMMANDBASE_H_

#include <Commands/Command.h>

namespace tator {

/**
 * The base for all commands.
 */
class CommandBase: public Command {
public:
	CommandBase();
	virtual ~CommandBase();

protected:
	virtual void Initialize() = 0;
	virtual void Execute() = 0;
	virtual bool IsFinished() = 0;
	virtual void End() = 0;
	virtual void Interrupted() = 0;

	// Place static subsystems pointers here

private:
	/**
	 * Initializes all static subsystem pointers
	 */
	void InitSubsystems();
};

} /* namespace tator */

#endif /* COMMANDBASE_H_ */
