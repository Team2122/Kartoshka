/**
 * @file Tester.cpp
 * @author Lee Bousfield
 * @date Feb 3, 2015
 */

#include "Tester.h"
#include "TestGroup.h"
#include "CommandBase.h"
#include "Subsystems/Drive.h"
#include "Subsystems/Shuttle.h"
#include "Subsystems/ToteFeed.h"
#include "Tests/SuccessfulTest.h"
#include "Tests/FailureTest.h"
#include "Tests/FingersTest.h"
#include "Tests/SpeedControllerEncoderTest.h"
#include "Tests/ShuttleHomeTest.h"
#include "Tests/ShuttlePistonsTest.h"
#include "Tests/ToteFeedTest.h"

namespace tator {

Tester* Tester::instance = nullptr;

Tester::Tester() {
	Test* leftDrive = new SpeedControllerEncoderTest("left drive",
			CommandBase::drive->driveL, CommandBase::drive->encoderL, .25, .25,
			.6);
	Test* rightDrive = new SpeedControllerEncoderTest("right drive",
			CommandBase::drive->driveR, CommandBase::drive->encoderR, .25, .25,
			.6);
	TestGroup* driveTests = new TestGroup("Drive", { leftDrive, rightDrive });
	Test* shuttleLift = new SpeedControllerEncoderTest("shuttle lift",
			CommandBase::shuttle->liftMotor, CommandBase::shuttle->liftEncoder,
			.25, -.30, 200);
	Test* shuttleHomeTest = new ShuttleHomeTest(CommandBase::shuttle->liftMotor,
			CommandBase::shuttle->lowerLimit, CommandBase::shuttle->upperLimit,
			CommandBase::shuttle->clampPiston);
	Test* toteFeedTest = new ToteFeedTest(CommandBase::toteFeed->rollers,
			CommandBase::toteFeed->backSensor, CommandBase::shuttle->toteSensor,
			CommandBase::shuttle->clampPiston);
	Test* fingersTest = new FingersTest(CommandBase::toteFeed->flappers,
			CommandBase::toteFeed->backSensor);
	TestGroup* shuttleTests = new TestGroup("Shuttle", { shuttleHomeTest,
			shuttleLift, toteFeedTest, fingersTest });

	Test* shuttleOpen = new ShuttleTest("ShuttleOpenTest", false, false)
	;
	Test* shuttleClamped = new ShuttleTest("ShuttleClampedTest", true, true)
	;
	Test* shuttleGrab = new ShuttleTest("ShuttleGrapTest", true, false)
	;
	TestGroup* shuttlePistonsTests = new TestGroup("Shuttle Pistons", {
			shuttleOpen, shuttleClamped, shuttleGrab });

	tests = new TestGroup("Tester", { driveTests, shuttleTests,
			shuttlePistonsTests });
}

void Tester::End() {
	tests->logger.Info("Results:");
	for (TestResult result : tests->results) {
		result.Log();
	}
}

void Tester::Initialize() {
	tests->Initialize();
}

void Tester::Execute() {
	tests->Execute();
}

void Tester::Interrupted() {
	tests->Interrupted();
	End();
}

Tester::~Tester() {
}

Tester* Tester::GetInstance() {
	if (instance == nullptr) {
		instance = new Tester();
	}
	return instance;
}

}
