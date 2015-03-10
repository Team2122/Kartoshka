/**
 * @file ArmShuttle.h
 * @author Daniele Moro
 * @date 2/13/2015
 */

#ifndef ARMSHUTTLE_H_
#define ARMSHUTTLE_H_

#include "CommandBase.h"
#include "Common/Kremlin.h"
#include "Subsystems/Shuttle.h"
#include "Subsystems/ToteFeed.h"

namespace tator {

class ArmShuttle: public CommandBase {
public:
	ArmShuttle(std::string name, YAML::Node config) :
			CommandBase(name) {
		stackToteCommand = Kremlin::Get("$StackTote");
		restackCommand = Kremlin::Get("$Restack");
		toteTickCount = 0;
		YAML::Node first = config["first"];
		firstRollerSpeed = first["rollerSpeed"].as<double>();
		firstFlapperSpeed = first["flapperSpeed"].as<double>();
		YAML::Node rest = config["rest"];
		restRollerSpeed = rest["rollerSpeed"].as<double>();
		restFlapperSpeed = rest["flapperSpeed"].as<double>();
		toteTicksRequired = config["toteTicks"].as<int>();
	}

	static std::string GetBaseName() {
		return "ArmShuttle";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		shuttle->OpenProngs();
		shuttle->ResetMaxToteCount();
		toteTickCount = 1;
	}

	void Execute() override {
		if (shuttle->IsTotePresent()) {
			toteTickCount++;
		} else {
			toteTickCount = 0;
		}
		if (toteTickCount >= toteTicksRequired && !stackToteCommand->IsRunning()
				&& shuttle->GetToteCount() < shuttle->GetMaxToteCount()) {
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

	bool IsFinished() override {
		return shuttle->GetToteCount() >= shuttle->GetMaxToteCount()
				&& toteTickCount >= toteTicksRequired; // 0 is 1, 4 is 5
	}

	void End() override {
		toteFeed->SetFlapperSpeed(0);
		if (stackToteCommand->IsRunning())
			stackToteCommand->Cancel();
		toteFeed->SetRollers(0);
		restackCommand->Start();
		CommandBase::End();
	}

	void Interrupted() override {
		toteFeed->SetFlapperSpeed(0);
		if (stackToteCommand->IsRunning())
			stackToteCommand->Cancel();
		toteFeed->SetRollers(0);
		CommandBase::Interrupted();
	}

private:
	Command* stackToteCommand, *restackCommand;
	double firstRollerSpeed, firstFlapperSpeed;
	double restRollerSpeed, restFlapperSpeed;
	int toteTicksRequired, toteTickCount;
};

}

#endif
