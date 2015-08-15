/**
 * @file CommandBase.h
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#ifndef COMMANDBASE_H_
#define COMMANDBASE_H_

#include <Commands/Command.h>
#include <yaml-cpp/yaml.h>
#include "Common/Logger.h"

namespace tator {

/**
 * The base for all commands. All subclasses must implement static std::string
 * GetBaseName() in order to work with Kremlin.
 */
template<typename T>
class CommandBase: public Command {
public:
	CommandBase(const std::string& name);
	virtual ~CommandBase();

	/**
	 * Initializes all static subsystem pointers
	 */
	static void InitSubsystems(YAML::Node subsystem);

	static T* robot;

protected:
	Logger log;

	virtual void Initialize() override;
	virtual void Execute() = 0;
	virtual bool IsFinished() = 0;
	virtual void End() override;
	virtual void Interrupted();
};

template<typename T>
T* CommandBase<T>::robot = nullptr;

template<typename T>
CommandBase<T>::CommandBase(const std::string& name) :
		Command(name.c_str()), log(name.c_str()) {
}

template<typename T>
CommandBase<T>::~CommandBase() {
}

template<typename T>
void CommandBase<T>::Initialize() {
	log.Info("%s has Initialized", GetName().c_str());
}

template<typename T>
void CommandBase<T>::End() {
	log.Info("%s has Ended", GetName().c_str());
}

template<typename T>
void CommandBase<T>::Interrupted() {
	log.Warn("%s has been Interrupted", GetName().c_str());
}

} /* namespace tator */

#endif /* COMMANDBASE_H_ */
