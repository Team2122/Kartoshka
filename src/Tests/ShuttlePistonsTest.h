/**
 * @file ShuttlePistonsTest.h
 * @author Zachary Snow
 * @date Mar 22, 2015
 */

#ifndef SHUTTLEPISTONSTEST_H_
#define SHUTTLEPISTONSTEST_H_

#include "Common/Tester/Test.h"
#include "Common/Hardware/FixedField.h"
#include <functional>

namespace tator {

class ShuttlePistonsTest: public Test {
private:
	enum class State {
		start, upOne, upTwo, top, drop, down, bottom, finished
	};

	State state;

public:
	ShuttlePistonsTest(std::string name, SpeedController* speedController,
			FixedField* lower, FixedField* upper, Solenoid* clamp,
			Solenoid* fingers, FixedField* toteSensor, Encoder* liftEncoder,
			bool shouldClamp, bool shouldRatchet) :
			Test(name), speedController(speedController), lower(lower), upper(
					upper), clamp(clamp), fingers(fingers), toteSensor(
					toteSensor), liftEncoder(liftEncoder), shouldClamp(
					shouldClamp), shouldRatchet(shouldRatchet) {
		state = State::start;
	}

protected:
	void Initialize() override {
		Test::Initialize();
		timer.Reset();
		timer.Start();
		state = State::start;
	}

	bool StateAfter(State nextState, std::function<bool()> condition) {
		if (condition()) {
			state = nextState;
			timer.Reset();
			return true;
		}
		return false;
	}

	bool StateAfterTime(State nextState, double time) {
		return StateAfter(nextState, [&]() {return timer.Get() > time;});
	}

	bool StateAfterSensor(State nextState, FixedField* sensor) {
		return StateAfter(nextState, [&]() {return sensor->Get();});
	}

	void Execute() override {
		switch (state) {

		case State::start:
			clamp->Set(!shouldClamp);
			StateAfterTime(State::upOne, .5);
			break;

		case State::upOne:
			speedController->Set(shouldClamp ? -.5 : -.25);
			this->StateAfter(State::upTwo,
					[&] {return liftEncoder->Get() >= 350;});
			break;

		case State::upTwo:
			clamp->Set(!true);
			this->StateAfterSensor(State::top, upper);
			break;

		case State::top:
			fingers->Set(!shouldRatchet);
			speedController->Set(-.1);
			if (this->StateAfterTime(State::drop, 0.5)) {
				if (toteSensor->Get()) {
					if (shouldClamp) {
						Error("We didn't grab that tote, man!");
					} else {
						Success("The tote stayed there");
					}
				} else {
					if (shouldClamp) {
						Success("Grabbed the tote");
					} else {
						Error("We grabbed that tote, man!");
					}
				}
			}
			break;

		case State::drop:
			speedController->Set(0.1);
			this->StateAfter(State::down,
					[&] {return liftEncoder->Get() <= 400;});
			break;

		case State::down:
			clamp->Set(!shouldClamp);
			this->StateAfterSensor(State::bottom, lower);
			break;

		case State::bottom:
			if (this->StateAfterTime(State::finished, 0.5)) {
				if (toteSensor->Get()) {
					if (shouldRatchet) {
						Error("The tote came of the ratchets!");
					} else {
						Success("The tote's at the bottom");
					}
				} else {
					if (shouldRatchet) {
						Success("Grabbed the tote");
					} else {
						Error("The totes still on the ratchets!");
					}
				}
			}
			break;

		default:
			break;
		}
	}

	bool IsFinished() override {
		return state == State::finished || timer.HasPeriodPassed(10.0);
	}

	void End() override {
		speedController->Set(0);
		Test::End();
	}

	void Interrupted() override {
		speedController->Set(0);
		Test::Interrupted();
	}

private:
	Timer timer;
	SpeedController* speedController;
	FixedField *lower, *upper;
	Solenoid* clamp;
	Solenoid* fingers;
	FixedField* toteSensor;
	Encoder* liftEncoder;

	bool shouldClamp;
	bool shouldRatchet;
};

#define ShuttleTest(testName, shouldClamp, shouldRatchet) \
	ShuttlePistonsTest(testName, CommandBase::shuttle->liftMotor, \
			CommandBase::shuttle->lowerLimit, CommandBase::shuttle->upperLimit, \
			CommandBase::shuttle->clampPiston, CommandBase::shuttle->fingersPiston, \
			CommandBase::shuttle->toteSensor, CommandBase::shuttle->liftEncoder, \
			shouldClamp, shouldRatchet) \

}

#endif /* SHUTTLEPISTONSTEST_H_ */
