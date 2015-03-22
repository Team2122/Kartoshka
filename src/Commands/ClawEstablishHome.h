#ifndef CLAWESTABLISHHOME_H_
#define CLAWESTABLISHHOME_H_

#include "CommandBase.h"
#include "Subsystems/Claw.h"

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
	void Execute() override {
		if (!claw->IsAtHome() && !hasEstablished) { // only establish once
			log.Error(
					"You tried to start the claw when it is not in the home position."
							" This has been decided as unsafe. Please fix this and restart the"
							" robot code if you would like claw functionality to be enabled");
			claw->DisableClaw();
		}
	}

	bool IsFinished() override {
		return true;
	}

	void End() override {
		if (!hasEstablished) {
			claw->ZeroLiftEncoder(); // only zero if we actually did something
		}
		hasEstablished = true;
		CommandBase::End();
	}

private:
	bool hasEstablished;
};

}

#endif
