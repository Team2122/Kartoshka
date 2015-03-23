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
#include "Tests/SuccessfulTest.h"
#include "Tests/FailureTest.h"
#include "Tests/SpeedControllerEncoderTest.h"
#include "Tests/ShuttleHomeTest.h"

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
			.25, -.25, 300);
	Test* shuttleHomeTest = new ShuttleHomeTest(CommandBase::shuttle->liftMotor,
			CommandBase::shuttle->lowerLimit, CommandBase::shuttle->upperLimit);
	TestGroup* shuttleTests = new TestGroup("Shuttle", { shuttleLift,
			shuttleHomeTest });
	tests = new TestGroup("Tester", { driveTests, shuttleTests });
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
