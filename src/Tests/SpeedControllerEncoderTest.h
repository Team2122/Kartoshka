/**
 * @file SpeedControllerEncoderTest.h
 * @author Alex Mikhalev
 * @date Mar 22, 2015
 */

#ifndef SPEEDCONTROLLERENCODERTEST_H_
#define SPEEDCONTROLLERENCODERTEST_H_

#include "Common/Test.h"
#include <WPILib.h>

namespace tator {

class SpeedControllerEncoderTest: public Test {
public:
	SpeedControllerEncoderTest(std::string name,
			SpeedController* speedController, Encoder* encoder, double time,
			double speed, double minimumEncoderRate) :
			Test(name), speedController(speedController), encoder(encoder), time(
					time), speed(speed), minimumEncoderRate(minimumEncoderRate) {

	}

	void Initialize() override {
		speedController->Set(speed);
		timer.Reset();
		timer.Start();
	}

	void Execute() override {

	}

	bool IsFinished() override {
		return timer.HasPeriodPassed(time);
	}

protected:
	void End() override {
		speedController->Set(speed);
		double rate = encoder->GetRate();
		if (rate / minimumEncoderRate < 1) {
			Error("The encoder was going at rate %f which is less than %f",
					rate, minimumEncoderRate);
		} else {
			Success("The encoder works");
		}
	}

	void Interrupted() override {
		speedController->Set(speed);
	}

private:
	Timer timer;
	SpeedController* speedController;
	Encoder* encoder;
	double time, speed, minimumEncoderRate;
};

}

#endif /* SPEEDCONTROLLERENCODERTEST_H_ */
