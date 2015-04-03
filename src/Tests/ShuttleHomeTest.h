/**
 * @file ShuttleHomeTest.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef SHUTTLEHOMETEST_H_
#define SHUTTLEHOMETEST_H_

#include "Common/Tester/Test.h"
#include "Common/Hardware/FixedField.h"

namespace tator {

class ShuttleHomeTest: public Test {
public:
	ShuttleHomeTest(SpeedController* speedController, FixedField* lower,
			FixedField* upper, Solenoid* clamp, Encoder* liftEncoder) :
			Test("ShuttleHomeTest"), speedController(speedController), lower(
					lower), upper(upper), clamp(clamp), liftEncoder(liftEncoder) {
		unclamped = false;
		finished = false;
		upperSensorTripped = false;
	}

protected:
	void Initialize() override {
		Test::Initialize();
		timer.Reset();
		timer.Start();
		unclamped = upperSensorTripped = finished = false;
		clamp->Set(true);
	}

	void Execute() override {
		double time = timer.Get();
		if (!unclamped) {
			if (time >= 1.0) {
				timer.Reset();
				unclamped = true;
			}
		} else if (!upperSensorTripped) {
			speedController->Set(-.30);
			if (liftEncoder->Get() >= 350) {
				clamp->Set(false);
			}
			if (time >= 5.0) {
				Error("Upper sensor was not tripped after 5 seconds");
				upperSensorTripped = true;
				timer.Reset();
			} else if (upper->Get()) {
				Success("Upper sensor tripped after %f seconds", time);
				upperSensorTripped = true;
				timer.Reset();
			}
		} else {
			speedController->Set(.15);
			if (liftEncoder->Get() <= 400) {
				clamp->Set(true);
			}
			if (time >= 6.0) {
				Error("Lower sensor was not tripped after 6 seconds falling");
				finished = true;
			} else if (lower->Get()) {
				Success("Lower sensor tripped after %f seconds", time);
				finished = true;
			}
		}
	}

	bool IsFinished() override {
		return timer.HasPeriodPassed(10.0) || finished;
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
	Encoder* liftEncoder;
	bool unclamped, upperSensorTripped, finished;
};

}

#endif /* SHUTTLEHOMETEST_H_ */
