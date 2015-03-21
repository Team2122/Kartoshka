/**
 * @file BumpBottomTote.h
 * @date Mar 20, 2015
 * @author Zachary Snow
 */

#include "Subsystems/ToteFeed.h"
#include "CommandBase.h"

#ifndef BUMPBOTTOMTOTE_H_
#define BUMPBOTTOMTOTE_H_

namespace tator {

class BumpBottomTote: public CommandBase {
public:
	BumpBottomTote(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(toteFeed);
		startTime = 0;
		time = config["time"].as<double>();
		rollerSpeed = config["rollerSpeed"].as<double>();
		reversed = false;
	}

	static std::string GetBaseName() {
		return "BumpBottomTote";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		reversed = false;
		startTime = Timer::GetFPGATimestamp();
	}

	void Execute() override {
		toteFeed->SetRollers(rollerSpeed * (reversed ? -1 : 1));
	}

	bool IsFinished() override {
		if (Timer::GetFPGATimestamp() > startTime + time) {
			if (reversed) {
				return true;
			}
			reversed = true;
			startTime = Timer::GetFPGATimestamp();
		}
		return false;
	}

	void End() override {
		CommandBase::End();
		toteFeed->SetRollers(0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		toteFeed->SetRollers(0);
	}

private:
	double rollerSpeed, time;
	double startTime;
	bool reversed;
};

}

#endif /* BUMPBOTTOMTOTE_H_ */
