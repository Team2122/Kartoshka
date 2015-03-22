/**
 * @file Tester.cpp
 * @author Lee Bousfield
 * @date Feb 3, 2015
 */

#include "Tester.h"
#include "TestGroup.h"
#include "CommandBase.h"
#include "Subsystems/Drive.h"
#include "Tests/SuccessfulTest.h"
#include "Tests/FailureTest.h"
#include "Tests/SpeedControllerEncoderTest.h"

namespace tator {

Tester* Tester::instance = nullptr;

Tester::Tester() {
	Test* leftDrive = new SpeedControllerEncoderTest("left drive",
			CommandBase::drive->driveL, CommandBase::drive->encoderL, 1, .25,
			1);
	Test* rightDrive = new SpeedControllerEncoderTest("right drive",
			CommandBase::drive->driveR, CommandBase::drive->encoderR, 1, .25,
			1);
	TestGroup* driveTests = new TestGroup("Drive", { leftDrive, rightDrive });
	tests = new TestGroup("Tester", { driveTests });
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
