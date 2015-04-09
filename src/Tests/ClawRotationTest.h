/**
 * @file ClawRotationTest.h
 * @author Alex Mikhalev
 * @date Apr 8, 2015
 */

#ifndef CLAWROTATIONTEST_H_
#define CLAWROTATIONTEST_H_

#include "Common/Tester/Test.h"

namespace tator {

class ClawRotationTest: public Test {
public:
	ClawRotationTest(AnalogPotentiometer* rotationEncoder) :
			Test("ClawRotationTest"), rotationEncoder(rotationEncoder) {
	}

protected:
	void Execute() override {
		double angle = rotationEncoder->Get();
		double distance = fabs(angle - 68);
		if (distance >= 2.5) {
			Error("The claw angle is off of what it should be. %f vs 68",
					angle);
		}
	}

	bool IsFinished() override {
		return true;
	}

private:
	AnalogPotentiometer* rotationEncoder;
};

}

#endif /* CLAWROTATIONTEST_H_ */
