/**
 * @file Tester.cpp
 * @author Lee Bousfield
 * @date Feb 3, 2015
 */

#include "Tester.h"
#include "TestGroup.h"
#include "Tests/SuccessfulTest.h"
#include "Tests/FailureTest.h"

namespace tator {

Tester* Tester::instance = nullptr;

Tester::Tester() :
		TestGroup("Tester", { new TestGroup("SimultaneousTestGroup", {
				new SuccessfulTest(), new FailureTest() }, true, false),
				new TestGroup("SequentialTestGroup", { new SuccessfulTest(),
						new FailureTest() }, false, false) }, false) {
}

void Tester::End() {
	logger.Info("Results:");
	for (TestResult result : results) {
		result.Log();
	}
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
