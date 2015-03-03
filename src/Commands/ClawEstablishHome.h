#ifndef CLAWESTABLISHHOME_H_
#define CLAWESTABLISHHOME_H_

#include "CommandBase.h"

namespace tator {

class ClawEstablishHome: public CommandBase {
public:
	ClawEstablishHome(std::string name, YAML::Node config) :
			CommandBase(name) {
		hasEstablished = false;
	}

	static std::string GetBaseName() {
		return "ClawEstablishHome";
	}
protected:
	bool hasEstablished;
	virtual void Initialize() {
		CommandBase::Initialize();
	}
	virtual void Execute() {
		if (!claw->IsHome() && !hasEstablished) {
			log.Error(
					"You tried to start the claw when it is not in the home position."
							" This has been decided as unsafe. Please fix this and restart the"
							" robot code if you would like claw functionality to be enabled");
			claw->DisableClaw();
		}
	}
	virtual bool IsFinished() {
		return true;
	}
	virtual void End() {
		if (!hasEstablished) {
			claw->ZeroLiftEncoder();
			Kremlin::Get("ClawRotationPick")->Start();
		}
		hasEstablished = true;
		CommandBase::End();
	}
	virtual void Interrupted() {
		CommandBase::Interrupted();
	}
};

}

#endif
