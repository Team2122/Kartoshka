/**
 * @file DisarmShuttle.h
 * @author Daniele Moro
 * @date 2/13/2015
 */

#ifndef DISARMSHUTTLE_H_
#define DISARMSHUTTLE_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class DisarmShuttle: public CommandBase {
public:
	DisarmShuttle(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "DisarmShuttle";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
		state = Shuttle::kDisarmed; //May use later if we need to add complexity
		ArmShuttlePointer = Kremlin::Get("ArmShuttle");
	}

	virtual void Execute() {
		if(ArmShuttlePointer->IsRunning()){
			ArmShuttlePointer->End();
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
	Command* ArmShuttlePointer;
};

}

#endif
