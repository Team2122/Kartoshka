/**
 * @file SpeedControllerEncoderTest.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef SPEEDCONTROLLERENCODERTEST_H_
#define SPEEDCONTROLLERENCODERTEST_H_

#include "Common/Tester/Test.h"
#include <WPILib.h>

namespace tator {

class SpeedControllerEncoderTest: public Test {
public:
	SpeedControllerEncoderTest(std::string name,
			SpeedController* speedController, Encoder* encoder, double time,
			double speed, double minimumEncoderRate) :
			Test("SpeedControllerEncoder " + name), speedController(
					speedController), encoder(encoder), time(time), speed(
					speed), minimumEncoderRate(minimumEncoderRate) {

	}

	void Initialize() override {
		timer.Reset();
		timer.Start();
	}

	void Execute() override {
		speedController->Set(speed);
	}

	bool IsFinished() override {
		return timer.HasPeriodPassed(time);
	}

protected:
	void End() override {
		speedController->Set(0);
		double rate = encoder->GetRate();
		if (rate / minimumEncoderRate < 1) {
			Error("The encoder was going at rate %f which is less than %f",
					rate, minimumEncoderRate);
		} else {
			Success("The encoder works at rate %f", rate);
		}
	}

	void Interrupted() override {
		speedController->Set(0);
	}

private:
	Timer timer;
	SpeedController* speedController;
	Encoder* encoder;
	double time, speed, minimumEncoderRate;
};

}

#endif /* SPEEDCONTROLLERENCODERTEST_H_ */
