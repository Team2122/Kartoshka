#ifndef CLAWESTABLISHHOME_H_
#define CLAWESTABLISHHOME_H_

#include "CommandBase.h"

namespace tator {

class ClawEstablishHome : public CommandBase {
public:
	ClawEstablishHome(std::string name, YAML::Node config)
		: CommandBase(name) {

	}

	static std::string GetBaseName() {
		return "ClawEstablishHome";
	}
protected:
	virtual void Initialize() {
		CommandBase::Initialize();
	}
	virtual void Execute() {
		if (!claw->IsHome()) {
			log.Error("You tried to start the claw when it is not in the home position."
					" This has been decided as unsafe. Please fix this and restart the"
					" robot code if you would like claw functionality to be enabled");
			claw->DisableClaw();
		}
	}
	virtual bool IsFinished() {
		return true;
	}
	virtual void End() {
		claw->ZeroLiftEncoder();
	}
	virtual void Interrupted() {
	}
};

}

#endif
