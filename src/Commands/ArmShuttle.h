/**
 * @file ArmShuttle.h
 * @author Daniele Moro
 * @date 2/13/2015
 */

#ifndef ARMSHUTTLE_H_
#define ARMSHUTTLE_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ArmShuttle: public CommandBase {
public:
	ArmShuttle(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "ArmShuttle";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
		state = Shuttle::kArmed; //Might use later if we need to add complexity
		StackToteCommandGroupPointer = Kremlin::Get("StackToteCommandGroup");
	}

	virtual void Execute() {
		if(shuttle->IsTotePresent() && !StackToteCommandGroupPointer->IsRunning() && shuttle->GetToteCount() < 5){
			StackToteCommandGroupPointer->Start();
		}
	}

	virtual bool IsFinished() {
		return false;
	}

	virtual void End() {
	}

	virtual void Interrupted() {
	}

protected:
	Shuttle::TotesCheckState state;
	Command* StackToteCommandGroupPointer;
};

}

#endif
