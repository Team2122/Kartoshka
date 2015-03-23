/**
 * @file ShuttleHomeTest.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef SHUTTLEHOMETEST_H_
#define SHUTTLEHOMETEST_H_

#include "Common/Test.h"
#include "Common/FixedField.h"
#include <WPILib.h>

namespace tator {

class ShuttleHomeTest: public Test {
public:
	ShuttleHomeTest(SpeedController* speedController, FixedField* lower,
			FixedField* upper) :
			Test("ShuttleHomeTest"), speedController(speedController), lower(
					lower), upper(upper) {
		finished = false;

	}

	void Initialize() override {
		timer.Reset();
		timer.Start();
		upperSensorTripped = finished = false;
	}

	void Execute() override {
		double time = timer.Get();
		if (!upperSensorTripped) {
			speedController->Set(-.25);
			if (time >= 3.0) {
				Error("Upper sensor was not tripped after 3 seconds");
				upperSensorTripped = true;
			} else if (upper->Get()) {
				Success("Upper sensor tripped after %f seconds", time);
				upperSensorTripped = true;
				timer.Reset();
			}
		} else {
			speedController->Set(0);
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
	bool upperSensorTripped, finished;
};

}

#endif /* SPEEDCONTROLLERENCODERTEST_H_ */
