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

protected:
	void Initialize() override {
		CommandBase::Initialize();
		toteFeed->SetRollers(rollerSpeed);
		currState = State::rolling;
	}

	void Execute() override {
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

	bool IsFinished() override {
		return currState == State::done;
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
	State currState;
	double rollerSpeed;
};

}

#endif /* RECIEVETOTE_H_ */
