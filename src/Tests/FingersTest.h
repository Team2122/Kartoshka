/**
 * @file FingersTest.h
 * @author Zachary Snow
 * @date Mar 23, 2015
 */

#ifndef FINGERSTEST_H_
#define FINGERSTEST_H_

#include "Common/Tester/Test.h"
#include "Common/Hardware/FixedField.h"

namespace tator {

class FingersTest: public Test {
public:
	FingersTest(SpeedController* speedController, FixedField* toteSensor) :
			Test("ShuttleHomeTest"), speedController(speedController), toteSensor(
					toteSensor) {
	}

	void Initialize() override {
		timer.Reset();
		timer.Start();
	}

	void Execute() override {
		speedController->Set(1.0);
	}

	bool IsFinished() override {
		return toteSensor->Get() || timer.HasPeriodPassed(1.0);
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
	FixedField* toteSensor;
};

}

#endif /* FINGERSTEST_H_ */
