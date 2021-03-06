/**
 * @file ToteDictator.h
 * @date Mar 20, 2015
 * @author Zachary Snow
 */

#ifndef TOTEDICTATOR_H_
#define TOTEDICTATOR_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"
#include "Subsystems/ToteFeed.h"
#include <cstring>

namespace tator {

class ToteDictator: public CommandBase {
public:
	ToteDictator(std::string name, YAML::Node config) :
			CommandBase(name) {
		recieveBottomTote = Kremlin::Get("RecieveBottomTote");
		bumpBottomTote = Kremlin::Get("BumpBottomTote");
		holdBottomTote = Kremlin::Get("HoldBottomTote");
		intakeTotes = Kremlin::Get("IntakeTotes");

		stackSequence = nullptr;
		restackSequence = nullptr;
		unstackSequence = nullptr;

		bottomSampleCount = 0;
		bottomSamplesRequired = config["samples"]["bottom"].as<int>();
		shuttleSampleCount = 0;
		shuttleSamplesRequired = config["samples"]["shuttle"].as<int>();
	}

	static std::string GetBaseName() {
		return "ToteDictator";
	}

protected:
	struct LogData {
		bool toteAtLift;
		bool toteAtRollers;
		int totesDesired;
		int totesHeld;
		int totesRatcheted;
	};

	LogData GetLogData() {
		return LogData { shuttle->HasToteAtShuttleBase(),
				toteFeed->GetBackSensor(), shuttle->GetDesiredTotes(),
				shuttle->GetTotesHeld(), shuttle->GetTotesRatcheted() };
	}

	void PrintLogData(LogData data) {
		log.Info("Tote at lift base: %s, tote at rollers: %s, "
				"totes desired: %d, totes held: %d, totes ratcheted: %d",
				data.toteAtLift ? "true" : "false",
				data.toteAtRollers ? "true" : "false", data.totesDesired,
				data.totesHeld, data.totesRatcheted);
	}

	void Initialize() override {
		CommandBase::Initialize();
		stackSequence = Kremlin::Get("$StackTote");
		restackSequence = Kremlin::Get("$Restack");
		unstackSequence = Kremlin::Get("$Unstack");
		fixJamSequence = Kremlin::Get("$FixJam");
		lastLogData = GetLogData();
	}

	void Execute() override {
		LogData logData = GetLogData();
		if (memcmp(&logData, &lastLogData, sizeof(LogData)) != 0) {
			PrintLogData(logData);
			lastLogData = logData;
		}
		// If any sequence is running
		if (stackSequence->IsRunning() || restackSequence->IsRunning()
				|| unstackSequence->IsRunning()
				|| fixJamSequence->IsRunning()) {
			return; // Now is not the time for logic
		}
		// If there is tote at the bottom of the bot
		if (toteFeed->GetBackSensor()) {
			// And we have not counted it yet
			if (shuttle->GetTotesHeld() - shuttle->GetTotesRatcheted() == 0) {
				bottomSampleCount++; // Increment the sample count
				// If we have waited long enough
				if (bottomSampleCount >= bottomSamplesRequired) {
					shuttle->SetTotesHeld(shuttle->GetTotesHeld() + 1); // Count it
					bumpBottomTote->Start(); // Realign it
				}
			}
		}
		// If we don't have a tote at the bottom
		else {
			// And we have not counted any totes yet
			if (shuttle->GetTotesHeld() - shuttle->GetTotesRatcheted() == 0) {
				// Intake the bottom tote
				recieveBottomTote->Start();
				// Reset the sample count
				bottomSampleCount = 0;
			}

			// And we do have at least one tote
			else {
				// And it's not being bumped
				if (!bumpBottomTote->IsRunning()) {
					// Apply tote hold power
					holdBottomTote->Start();
				}
			}
		}

		// If we have a tote at the bottom of the lift
		if (shuttle->HasToteAtShuttleBase()) {
			// And we haven't counted it yet
			if (shuttle->GetTotesHeld() == shuttle->GetTotesRatcheted() + 1) {
				shuttleSampleCount++; // Increment sample count
				// If we've waited long enough
				if (shuttleSampleCount >= shuttleSamplesRequired) {
					shuttle->SetTotesHeld(shuttle->GetTotesHeld() + 1); // Count it
				}
			}
		}

		// If we don't have a tote at the shuttle base
		else {
			// Reset our sampling counter
			shuttleSampleCount = 0;
		}

		// If we have fewer totes than we want
		if (shuttle->GetTotesHeld() < shuttle->GetDesiredTotes()) {
			// And we already have one at the bottom
			if (shuttle->GetTotesHeld() - shuttle->GetTotesRatcheted() > 0) {
				// Start intaking
				intakeTotes->Start();

				// And if we have unratcheted totes
				if (shuttle->GetTotesHeld() - shuttle->GetTotesRatcheted()
						> 1) {
					// And we aren't already stacking
					if (!stackSequence->IsRunning()) {
						// Put it on the ratchets
						stackSequence->Start();
					}
				}
			}
		}

		// If we have at greater than or equal to the number of totes we want
		else {
			// And we have totes on the ratchets
			if (shuttle->GetTotesRatcheted() > 0) {
				// Act like they aren't there anymore
				shuttle->ZeroTotesRatcheted();
				// And if we have a tote at the shuttle base
				if (shuttle->HasToteAtShuttleBase()) {
					// Restack everything
					restackSequence->Start();
				}
				// And we don't have a tote at the shuttle base
				else {
					// Unstack everything
					unstackSequence->Start();
				}
			}
			// Stop the intake sequence
			this->Cancel();
		}
	}

	bool IsFinished() override {
		return false;
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		recieveBottomTote->Cancel();
		intakeTotes->Cancel();
	}

private:
	Command* recieveBottomTote;
	Command* bumpBottomTote;
	Command* holdBottomTote;
	Command* intakeTotes;

	Command* stackSequence;
	Command* restackSequence;
	Command* unstackSequence;
	Command* fixJamSequence;

	int bottomSampleCount;
	int bottomSamplesRequired;
	int shuttleSampleCount;
	int shuttleSamplesRequired;
	LogData lastLogData;
};

}

#endif /* TOTEDICTATOR_H_ */
