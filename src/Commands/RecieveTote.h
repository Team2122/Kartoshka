/**
 * @file RecieveTote.h
 * @date Jan 24, 2015
 * @author Lee Bousfield
 */

#include "Subsystems/ToteFeed.h"
#include "CommandBase.h"

#ifndef RECIEVETOTE_H_
#define RECIEVETOTE_H_

namespace tator {

class RecieveTote: public CommandBase {
public:

	enum class State {
		rolling, done
	};

	RecieveTote(std::string name, YAML::Node config) :
			CommandBase(name) {
		rollerSpeed = config["rollerSpeed"].as<double>();
		currState = State::rolling;
	}

	static std::string GetBaseName() {
		return "RecieveTote";
	}

	void Initialize() {
		CommandBase::Initialize();
		toteFeed->SetRollers(rollerSpeed);
		currState = State::rolling;
	}

	void Execute() {
		switch (currState) {
		case State::rolling:
			if (toteFeed->GetBackSensor()) {
				log.Info("Back beam triggered, stopping rollers");
				currState = State::done;
			} else {
				break;
			}
			// no break
		default:
			break;
		}
	}

	bool IsFinished() {
		return currState == State::done;
	}

	void End() {
		CommandBase::End();
		toteFeed->SetRollers(0);
	}

	void Interrupted() {
		CommandBase::Interrupted();
		toteFeed->SetRollers(0);
	}
private:
	State currState;
	double rollerSpeed;
};

}

#endif /* RECIEVETOTE_H_ */
