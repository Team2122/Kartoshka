/**
 * @file ToteDirector.h
 * @date Mar 20, 2015
 * @author Zachary Snow
 */

#ifndef TOTEDIRECTOR_H_
#define TOTEDIRECTOR_H_

#include "CommandBase.h"
#include "Subsystems/Shuttle.h"
#include "Subsystems/ToteFeed.h"

namespace tator {

class ToteDirector: public CommandBase {
public:
	ToteDirector(std::string name, YAML::Node config) :
			CommandBase(name) {
		recieveBottomTote = Kremlin::Get("RecieveBottomTote");
		bumpBottomTote = Kremlin::Get("BumpBottomTote");
		holdBottomTote = Kremlin::Get("HoldBottomTote");
		intakeTotes = Kremlin::Get("IntakeTotes");

		stackSequence = Kremlin::Get("$StackTote");
		restackSequence = Kremlin::Get("$Restack");
		unstackSequence = Kremlin::Get("$Unstack");

		sampleCount = 0;
		samplesRequired = config["samples"].as<int>();
	}

	static std::string GetBaseName() {
		return "ToteDirector";
	}

protected:
	void Execute() override {
		// If there is tote at the bottom of the bot
		if (toteFeed->GetBackSensor()) {
			// And we have not counted it yet
			if (shuttle->GetTotesHeld() == 0) {
				shuttle->SetTotesHeld(1); // Count it
				bumpBottomTote->Start(); // Realign it
			}
		}
		// If we don't have a tote at the bottom
		else {
			// And we have not counted any totes yet
			if (shuttle->GetTotesHeld() == 0) {
				// Intake the bottom tote
				recieveBottomTote->Start();
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
				sampleCount++; // Increment sample count
				// If we've waited long enough
				if (sampleCount == samplesRequired) {
					shuttle->SetTotesHeld(shuttle->GetTotesHeld() + 1); // Count it
				}
			}
		}

		// If we don't have a tote at the shuttle base
		else {
			// Reset our sampling counter
			sampleCount = 0;
		}

		// If we have fewer totes than we want
		if (shuttle->GetTotesHeld() < shuttle->GetDesiredTotes()) {
			// And we already have one at the bottom
			if (shuttle->GetTotesHeld() > 0) {
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

		// If we have exactly the number of totes we want
		else if (shuttle->GetDesiredTotes() == shuttle->GetTotesHeld()) {
			// And we have totes on the ratchets
			if (shuttle->GetTotesRatcheted() > 0) {
				// And if we have a tote at the shuttle base
				if (shuttle->HasToteAtShuttleBase()) {
					// Act like they aren't there anymore
					shuttle->ZeroTotesRatcheted();
					// Restack everything
					restackSequence->Start();
					// Stop the intake sequence
					this->Cancel();
				}

				// And if they are all on the ratchets
				else if (shuttle->GetTotesHeld()
						== shuttle->GetTotesRatcheted() + 1) {
					// Act like they aren't there anymore
					shuttle->ZeroTotesRatcheted();
					// Unstack everything
					unstackSequence->Start();
					// Stop the intake sequence
					this->Cancel();
				}
			}
		}

		// If we have more than the number of totes we want
		else {
			// And we have totes on the ratchets
			// And we don't have a tote at the shuttle base
			if (shuttle->GetTotesRatcheted() > 0
					&& !toteFeed->GetBackSensor()) {
				// Act like they aren't there anymore
				shuttle->ZeroTotesRatcheted();
				// Unstack everything
				unstackSequence->Start();
				// Stop the intake sequence
				this->Cancel();
			}
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

	int sampleCount;
	int samplesRequired;
};

}

#endif /* TOTEDIRECTOR_H_ */
