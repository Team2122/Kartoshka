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
		isBumping = false;

		YAML::Node first = config["first"];
		firstRollerSpeed = first["rollerSpeed"].as<double>();
		firstFlapperSpeed = first["flapperSpeed"].as<double>();

		YAML::Node rest = config["rest"];
		restRollerSpeed = rest["rollerSpeed"].as<double>();
		restFlapperSpeed = rest["flapperSpeed"].as<double>();

		YAML::Node bump = config["bump"];
		bumpRollerSpeed = bump["rollerSpeed"].as<double>();
		bumpFlapperSpeed = bump["flapperSpeed"].as<double>();

		bumpTime = config["bumpTime"].as<double>();
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
		isBumping = false;
		timer.Reset();
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
		bool doneBumping = timer.Get() >= bumpTime;
		bool hasFirstTote = toteFeed->GetBackSensor();
		if (isBumping && !doneBumping) {
			toteFeed->SetRollers(bumpRollerSpeed);
			toteFeed->SetFlapperSpeed(bumpFlapperSpeed);
		} else if (hasFirstTote && !isBumping) {
			isBumping = true;
			timer.Start();
		} else if (hasFirstTote && doneBumping) {
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
		timer.Stop();
		CommandBase::End();
	}

	void Interrupted() override {
		toteFeed->SetFlapperSpeed(0);
		if (stackToteCommand->IsRunning())
			stackToteCommand->Cancel();
		toteFeed->SetRollers(0);
		timer.Stop();
		CommandBase::Interrupted();
	}

private:
	Command* stackToteCommand, *restackCommand;
	double firstRollerSpeed, firstFlapperSpeed;
	double restRollerSpeed, restFlapperSpeed;
	double bumpRollerSpeed, bumpFlapperSpeed;
	int toteTicksRequired, toteTickCount;
	double bumpTime;
	bool isBumping;
	Timer timer;
};

}

#endif
