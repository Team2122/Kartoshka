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
#include "Tests/SpeedControllerEncoderTest.h"
#include "Tests/ShuttleHomeTest.h"
#include "Tests/ShuttlePistonsTest.h"
#include "Tests/ToteFeedTest.h"

namespace tator {

Tester* Tester::instance = nullptr;

Tester::Tester() :
		log("Tester") {
	testMode = TestMode::disabled;
	joy = Joystick::GetStickForPort(0);
	manualTester = ManualTester::GetInstance();
	tests = std::unique_ptr<TestGroup>(new TestGroup("Tester", { }));
}

Tester::~Tester() {
}

void Tester::CreateTests() {
	Test* leftDrive = new SpeedControllerEncoderTest("left drive",
			CommandBase::drive->driveL, CommandBase::drive->encoderL, .25, .25,
			.6);
	Test* rightDrive = new SpeedControllerEncoderTest("right drive",
			CommandBase::drive->driveR, CommandBase::drive->encoderR, .25, .25,
			.6);
	TestGroup* driveTests = new TestGroup("Drive", { leftDrive, rightDrive });
	this->AddTest(driveTests);

	Test* shuttleLift = new SpeedControllerEncoderTest("shuttle lift",
			CommandBase::shuttle->liftMotor, CommandBase::shuttle->liftEncoder,
			.25, -.25, 150);
	Test* shuttleHomeTest = new ShuttleHomeTest(CommandBase::shuttle->liftMotor,
			CommandBase::shuttle->lowerLimit, CommandBase::shuttle->upperLimit,
			CommandBase::shuttle->clampPiston,
			CommandBase::shuttle->liftEncoder);
	Test* toteFeedTest = new ToteFeedTest(CommandBase::toteFeed->rollers,
			CommandBase::toteFeed->flappers, CommandBase::toteFeed->backSensor,
			CommandBase::shuttle->toteSensor,
			CommandBase::shuttle->clampPiston);
	TestGroup* shuttleTests = new TestGroup("Shuttle", { shuttleHomeTest,
			shuttleLift, toteFeedTest });
	this->AddTest(shuttleTests);

	Test* shuttleOpen = new ShuttleTest("ShuttleOpenTest", false, false);
	Test* shuttleClamped = new ShuttleTest("ShuttleClampedTest", true, true);
	Test* shuttleGrab = new ShuttleTest("ShuttleGrapTest", true, false);
	TestGroup* shuttlePistonsTests = new TestGroup("Shuttle Pistons", {
			shuttleOpen, shuttleClamped, shuttleGrab });
	this->AddTest(shuttlePistonsTests);
}

void Tester::Initialize() {
	log.Info("Running the manual tester");
	log.Info("Press START (10) to switch to the automated Tester");
	testMode = TestMode::manual;
}

void Tester::Run() {
	switch (testMode) {
	case TestMode::manual:
		manualTester->Execute();
		if (joy->GetRawButton(10)) {
			log.Info("Running automated tests");
			manualTester->StopTesting();
			tests->Initialize();
			testMode = TestMode::autonomous;
		}
		break;
	case TestMode::autonomous:
		tests->Execute();
		if (tests->IsFinished()) {
			tests->End();
			this->PrintResults();
			testMode = TestMode::finished;
		}
		break;
	case TestMode::finished:
	default:
		break;
	}
}

void Tester::Disable() {
	switch (testMode) {
	case TestMode::autonomous:
		tests->Interrupted();
		PrintResults();
		break;
	case TestMode::manual:
		manualTester->StopTesting();
		break;
	default:
		break;
	}
	testMode = TestMode::disabled;
}

void Tester::PrintResults() {
	log.Info("Results:");
	for (TestResult result : tests->results) {
		result.Log();
	}
	tests->ClearResults();
}

void Tester::AddTest(Test* test) {
	tests->AddTest(test);
}

Tester* Tester::GetInstance() {
	if (instance == nullptr) {
		instance = new Tester();
	}
	return instance;
}

}
