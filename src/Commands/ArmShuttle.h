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
		stackToteCommand = Kremlin::Get("StackTote");
		firstRollerSpeed = config["first"]["rollerSpeed"].as<double>();
		firstFlapperSpeed = config["first"]["flapperSpeed"].as<double>();
		restRollerSpeed = config["rest"]["rollerSpeed"].as<double>();
		restFlapperSpeed = config["rest"]["flapperSpeed"].as<double>();
	}

	static std::string GetBaseName() {
		return "ArmShuttle";
	}

	virtual void Initialize() {
		CommandBase::Initialize();
		shuttle->OpenProngs();
	}

	virtual void Execute() {
		if (shuttle->IsTotePresent() && !stackToteCommand->IsRunning()
				&& shuttle->GetToteCount() < 4) {
			log.Info("Sensor triggered. Stacking totes...");
			stackToteCommand->Start();
		}
		if (toteFeed->GetBackSensor()) {
			toteFeed->SetRollers(restRollerSpeed);
			toteFeed->SetFlapperSpeed(restFlapperSpeed);
		} else {
			toteFeed->SetRollers(firstRollerSpeed);
			toteFeed->SetFlapperSpeed(firstFlapperSpeed);
		}
	}

	virtual bool IsFinished() {
		return shuttle->GetToteCount() >= 4 && shuttle->IsTotePresent(); // 0 is 1, 4 is 5
	}

	virtual void End() {
		toteFeed->SetFlapperSpeed(0);
		if (stackToteCommand->IsRunning())
			stackToteCommand->Cancel();
		toteFeed->SetRollers(0);
		CommandBase::End();
	}

	virtual void Interrupted() {
		toteFeed->SetFlapperSpeed(0);
		if (stackToteCommand->IsRunning())
			stackToteCommand->Cancel();
		toteFeed->SetRollers(0);
		CommandBase::Interrupted();
	}

protected:
	Command* stackToteCommand;
	double firstRollerSpeed, firstFlapperSpeed;
	double restRollerSpeed, restFlapperSpeed;
};

}

#endif
