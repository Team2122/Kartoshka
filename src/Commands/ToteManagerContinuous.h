#ifndef TOTEMANAGERCONTINUOUS_H_
#define TOTEMANAGERCONTINUOUS_H_

#include "CommandBase.h"

namespace tator {
class ToteManagerContinuous: public CommandBase {
public:
	ToteManagerContinuous(std::string name, YAML::Node config):
			CommandBase(name) {
		targetTotes = 0;
		feeding = false;
		bumpTimer = new Timer();
		toteDetectionTicks = config["toteDetectionTicks"].as<int>();
	}

	~ToteManagerContinuous() {
		delete bumpTimer;
	}

	std::string GetName() {
		return "ToteManagerContinuous";
	}

	void Feed() {
		wantedAction = Action::feeding;
		if (action == Action::passive) {
			action = Action::feeding;
		}
	}

public:
	void StopAllTheThings(); // For lack of .cpp

protected:
	void Unload() {
		wantedAction = Action::unloading;
		if (action != Action::restacking) {
			if (action != Action::passive) {
				StopAllTheThings();
			}
			action = Action::unloading;
		}
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		targetTotes = 0;
		feeding = false;
	}

	void StopAllTheThings() {
		toteFeed->SetRollers(0);
		toteFeed->SetFlapperSpeed(0);
		shuttle->CloseProngs();
		shuttle->SetShuttleSpeed(shuttle::Speed::kStop);
	}

	void Execute() override {
		if (targetTotes == 0) {
			return;
		}
		int totesHeld = shuttle->GetTotesHeld();
		int totesRatcheted = shuttle->GetTotesRatcheted();
		if (action == Action::feeding) {
			if (totesHeld == 0) {
				if (bumpTicks >= totalBumpTicks) {
					shuttle->SetTotesHeld(1);
					toteFeed->SetRollers(rollerSpeedHoldTote);
					bumpTicks = 0;
					state = State::resting;
				} else if (state == State::bumping) {
					bumpTicks++;
				} else if (toteFeed->GetBackSensor()) {
					toteTicks++;
					if (toteTicks >= toteDetectionTicks) {
						state = State::bumping;
						bumpTicks = 0;
						toteTicks = 0;
						toteFeed->SetRollers(rollerSpeedBump);
					}
				} else {
					toteFeed->SetRollers(rollerSpeedRollerbedIntake);
					toteFeed->SetFlapperSpeed(flapperSpeedRollerbedIntake);
					toteTicks = 0;
				}
			} else if (targetTotes > totesHeld) {
				// We have a tote in the rollerbed, and we need more
				shuttle->SetFingersPiston(Shuttle::kHeld);
				shuttle->OpenProngs();
				toteFeed->SetFlapperSpeed(flapperSpeedLowerLiftIntake);
				if (state == State::ratchetingTote) {
					bool atUpper = RampShuttleTo(upperTicks);
					if (atUpper) {
						toteTicks++;
						if (toteTicks >= toteDetectionTicks) {
							shuttle->OpenProngs();
							shuttle->UpdateTotesRatcheted();
							state = State::loweringShuttle;
						}
					}
				} else if (state == State::loweingShuttle) {

				} else if (shuttle->IsTotePresent()) {
					toteTicks++;
					if (toteTicks >= toteDetectionTicks) {
						toteTicks = 0;
						shuttle->CloseProngs();
						shuttle->SetTotesHeld(totesHeld + 1);
						state = State::ratchetingTote;
					}
				}
			} else {
				action = Action::passive;
				state = State::resting;
			}
		} else if (action == Action::passive) {
			if (totesHeld > 0) {
				toteFeed->SetRollers(rollerSpeedHoldTote);
			} else {
				toteFeed->SetRollers(0);
			}
			ticks = 0;
			if (state == State::loweringShuttle) {
				if (RampShuttleTo(lowerTicks)) {
					state = State::resting;
					wantedAction = Action::passive;
				}
			} else {
				if (totesHeld - totesRatcheted < targetTotes && totesHeld >= targetTotes) {
					action = Action::restacking;
				}
			}
			toteFeed->SetFlapperSpeed(0);
			shuttle->SetShuttleSpeed(Shuttle::Speed::kStop);
		} else if (action == Action::stop) {
			StopAllTheThings();
		}
	}

	void End() override {
		CommandBase::End();
	}

	void Interrupted() override {
		CommandBase::Interrupted();
	}

private:
	enum class Action {
		feeding, unloading, passive, restacking, stop
	}

	enum class State {
		bumping, ratchetingTote, loweringShuttle
	}

	int targetTotes; ///< The target number of totes for the next snack
	Action action; ///< The current action, see the enum for more details
	int totalBumpTicks; ///< Ticks to bump for
	int ticks; ///< A general tick counter, use case differs by code section
	State state; ///< The current state, see the enum for more details
	Action wantedAction; ///< The wanted action, used if currently re/unstacking
};
}

#endif /* TOTEMANAGERCONTINUOUS_H_ */

