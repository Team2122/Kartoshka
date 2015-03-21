/**
 * @file ShuttleHold.h
 * @author Alex Mikhalev
 * @date March 21, 2015
 */

#ifndef SHUTTLEHOLD_H_
#define SHUTTLEHOLD_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class ShuttleHold: public CommandBase {
public:
	ShuttleHold(std::string name, YAML::Node config) :
			CommandBase(name) {
		speed = config["speed"].as<double>();
		time = config["time"].as<double>();
		Requires(shuttle);
	}

	static std::string GetBaseName() {
		return "ShuttleHold";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		timer.Reset();
		timer.Start();
	}

	void Execute() override {
		shuttle->SetShuttleSpeed(speed);
	}

	bool IsFinished() override {
		return timer.Get() >= time;
	}

	void End() override {
		CommandBase::End();
		timer.Stop();
		shuttle->SetShuttleSpeed(0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		timer.Stop();
		shuttle->SetShuttleSpeed(0);
	}

private:
	Timer timer;
	double speed, time;
};

}

#endif /* SHUTTLEHOLD_H_ */
