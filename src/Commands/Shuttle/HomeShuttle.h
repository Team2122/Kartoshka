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
		Requires(shuttle);
		speed = config["speed"].as<double>();
	}

	static std::string GetBaseName() {
		return "HomeShuttle";
	}

protected:
	void Initialize() override {
		shuttle->SetShuttleSpeed(speed);
		CommandBase::Initialize();
	}

	void Execute() override {
		if (shuttle->GetLimit() == Shuttle::kUpper) {
			log.Error("We have big problems. The shuttle went up when we "
					"thought it should go down");
			this->Cancel();
		}
	}

	bool IsFinished() override {
		return shuttle->GetLimit() == Shuttle::kLower;
	}

	void End() override {
		shuttle->ResetEncoder();
		shuttle->SetShuttleSpeed(Shuttle::kHold);
		CommandBase::End();
	}

	void Interrupted() override {
		shuttle->SetShuttleSpeed(Shuttle::kHold);
		CommandBase::Interrupted();
	}

private:
	double speed;
};

}

#endif
