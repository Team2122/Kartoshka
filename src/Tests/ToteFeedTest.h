/**
 * @file ToteFeedTest.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef TOTEFEEDTEST_H_
#define TOTEFEEDTEST_H_

#include "Common/Tester/Test.h"
#include "Common/Hardware/FixedField.h"

namespace tator {

class ToteFeedTest: public Test {
public:
	ToteFeedTest(SpeedController* rollers, SpeedController* flappers,
			FixedField* lower, FixedField* upper, Solenoid* clamp) :
			Test("ShuttleHomeTest"), rollers(rollers), flappers(flappers), lower(
					lower), upper(upper), clamp(clamp) {
		rolledAway = finished = false;
	}

protected:
	void Initialize() override {
		Test::Initialize();
		timer.Reset();
		timer.Start();
		rolledAway = finished = false;
		clamp->Set(true);
	}

	void Execute() override {
		double time = timer.Get();
		if (!rolledAway) {
			rollers->Set(-.25);
			flappers->Set(-.25);
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
			rollers->Set(.25);
			flappers->Set(.25);
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

	void End() override {
		rollers->Set(0);
		flappers->Set(0);
		clamp->Set(false);
		Test::End();
	}

	void Interrupted() override {
		rollers->Set(0);
		flappers->Set(0);
		clamp->Set(false);
		Test::Interrupted();
	}

private:
	Timer timer;
	SpeedController* rollers;
	SpeedController* flappers;
	FixedField *lower, *upper;
	Solenoid* clamp;
	bool rolledAway, finished;
};

}

#endif /* TOTEFEEDTEST_H_ */
