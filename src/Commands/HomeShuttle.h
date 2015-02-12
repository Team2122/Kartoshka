/**
 * @file HomeShuttle.h
 * @author Alex Mikhalev
 * @date 2/8/2015
 */

#ifndef HOMESHUTTLE_H_
#define HOMESHUTTLE_H_

#include "CommandBase.h"

#include "Subsystems/Shuttle.h"

namespace tator {

class HomeShuttle: public CommandBase {
public:
	HomeShuttle(std::string name, YAML::Node config) :
			CommandBase(name) {
	}

	static std::string GetBaseName() {
		return "HomeShuttle";
	}

	virtual void Initialize() {
		shuttle->SetShuttleSpeed(Shuttle::kDown);
		CommandBase::Initialize();
	}

	virtual void Execute() {
	}

	virtual bool IsFinished() {
		if (shuttle->IsStalled()) {
			log.Error("The shuttle has stalled while homing");
			Cancel();
			return true;
		}
		switch (shuttle->GetLimit()) {
		case Shuttle::kUpper:
			log.Error("We have big problems. The shuttle went up when we "
					"thought it should go down");
			Cancel();
			return true;
		case Shuttle::kLower:
			return true;
		default:
			return false;
		}
	}

	virtual void End() {
		shuttle->ResetEncoder();
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		CommandBase::End();
	}

	virtual void Interrupted() {
		shuttle->SetShuttleSpeed(Shuttle::kStop);
		CommandBase::Interrupted();
	}

protected:
};

}

#endif
