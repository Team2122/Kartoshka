/**
 * @file ShuttleHomeTest.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef SHUTTLEHOMETEST_H_
#define SHUTTLEHOMETEST_H_

#include "Common/Tester/Test.h"
#include "Common/Hardware/FixedField.h"
#include <WPILib.h>

namespace tator {

class ShuttleHomeTest: public Test {
public:
	ShuttleHomeTest(SpeedController* speedController, FixedField* lower,
			FixedField* upper, Solenoid* clamp) :
			Test("ShuttleHomeTest"), speedController(speedController), lower(
					lower), upper(upper), clamp(clamp) {
		unclamped = false;
		finished = false;
		upperSensorTripped = false;

	}

	void Initialize() override {
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
			speedController->Set(-.25);
			if (time >= 1.5) {
				clamp->Set(false);
			}
			if (time >= 3.0) {
				Error("Upper sensor was not tripped after 3 seconds");
				upperSensorTripped = true;
				timer.Reset();
			} else if (upper->Get()) {
				Success("Upper sensor tripped after %f seconds", time);
				upperSensorTripped = true;
				timer.Reset();
			}
		} else {
			speedController->Set(0);
			if (time >= 1.5) {
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

protected:
	void End() override {
		speedController->Set(0);
	}

	void Interrupted() override {
		speedController->Set(0);
	}

private:
	Timer timer;
	SpeedController* speedController;
	FixedField *lower, *upper;
	Solenoid* clamp;
	bool unclamped, upperSensorTripped, finished;
};

}

#endif /* SHUTTLEHOMETEST_H_ */
