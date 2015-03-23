/**
 * @file ToteFeedTest.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef TOTEFEEDTEST_H_
#define TOTEFEEDTEST_H_

#include "Common/Tester/Test.h"
#include "Common/Hardware/FixedField.h"
#include <WPILib.h>

namespace tator {

class ToteFeedTest: public Test {
public:
	ToteFeedTest(SpeedController* speedController, FixedField* lower,
			FixedField* upper, Solenoid* clamp) :
			Test("ShuttleHomeTest"), speedController(speedController), lower(
					lower), upper(upper), clamp(clamp) {
		rolledAway = finished = false;

	}

	void Initialize() override {
		timer.Reset();
		timer.Start();
		rolledAway = finished = false;
		clamp->Set(true);
	}

	void Execute() override {
		double time = timer.Get();
		if (!rolledAway) {
			speedController->Set(-.25);
			if (time >= 2.0) {
				if (lower->Get() && upper->Get()) {
					Error("Sensor still tripped after 2 seconds");
				} else if (lower->Get()) {
					Error("Lower sensor still tripped after 2 seconds");
				} else {
					Error("Upper sensor still tripped after 2 seconds");
				}
				rolledAway = true;
				timer.Reset();
			} else if (!lower->Get() && !upper->Get()) {
				Success("Both sensors not tripped after %f seconds", time);
				rolledAway = true;
				timer.Reset();
			}
		} else {
			speedController->Set(.25);
			if (time >= 2.0) {
				if (!lower->Get() && !upper->Get()) {
					Error("Sensors not tripped after 2 seconds");
				} else if (!lower->Get()) {
					Error("Lower sensor still not tripped after 2 seconds");
				} else {
					Error("Upper sensor still not tripped after 2 seconds");
				}
				finished = true;
			} else if (lower->Get() && upper->Get()) {
				Success("Sensors tripped after %f seconds", time);
				finished = true;
			}
		}
	}

	bool IsFinished() override {
		return timer.HasPeriodPassed(5.0) || finished;
	}

protected:
	void End() override {
		speedController->Set(0);
		clamp->Set(false);
	}

	void Interrupted() override {
		speedController->Set(0);
		clamp->Set(false);
	}

private:
	Timer timer;
	SpeedController* speedController;
	FixedField *lower, *upper;
	Solenoid* clamp;
	bool rolledAway, finished;
};

}

#endif /* TOTEFEEDTEST_H_ */
