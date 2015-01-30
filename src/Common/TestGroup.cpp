/**
 * @file TestGroup.cpp
 * @author Lee Bousfield
 * @date Jan 31, 2015
 */

#include "TestGroup.h"
#include "TestResult.h"
#include "Tester.h"
#include "Test.h"


namespace tator {
TestGroup::TestGroup(const char* name, std::vector<Test*> tests,
		bool simultaneous, bool stopOnError):
			Test(name){
	this->name = name;
	for (Test* test : tests) {
		this->tests.push_back(new TestData{test, false, false});
	}
	this->simultaneous = simultaneous;
	this->stopOnError = stopOnError;
	this->currTest = this->tests.end();
}

TestGroup::~TestGroup() {
}

void TestGroup::Initialize() {
	if (simultaneous) {
		for (TestData* testPair : tests) {
			if (!testPair->hasStarted) {
				logger.Info("Initializing %s asynchronously", testPair->test->GetName().c_str());
				testPair->hasStarted = true;
				testPair->test->Initialize();
			}
		}
	} else {
		currTest = tests.begin();
		logger.Info("Initializing %s", (*currTest)->test->GetName().c_str());
		(*currTest)->hasStarted = true;
		(*currTest)->test->Initialize();
	}
}

void TestGroup::Execute() {
	if (simultaneous) {
		for (TestData* testPair : tests) {
			if (!testPair->isFinished) {
				testPair->test->Execute();
				if (testPair->test->IsFinished()) {
					testPair->isFinished = true;
					HandleFinishedTest(testPair->test);
					bool allDone = true;
					for (TestData* currTestData : tests) {
						if (!currTestData->isFinished) {
							allDone = false;
						}
					}
					if (allDone) {
						End();
					}
				}
			}
		}
	} else if (currTest != tests.end()) {
		(*currTest)->test->Execute();
		if ((*currTest)->test->IsFinished()) {
			(*currTest)->isFinished = true;
			HandleFinishedTest((*currTest)->test);
			currTest++;
			if (currTest == tests.end()) {
				End();
			}
		}
	}
}

bool TestGroup::IsFinished() {
	if (simultaneous) {
		for (TestData* testPair : tests) {
			if (!testPair->isFinished) {
				return false;
			}
		}
		return true;
	} else {
		return currTest == tests.end();
	}
}

void TestGroup::End() {
	// Nothing needs done as when this is finished everything is already ended
}

void TestGroup::Interrupted() {
	if (simultaneous) {
		for (TestData* testPair : tests) {
			if (!testPair->isFinished && testPair->hasStarted) {
				testPair->test->Interrupted();
				HandleFinishedTest(testPair->test, true);
				testPair->isFinished = true;
			}
		}
	} else {
		if (currTest != tests.end() && (*currTest)->hasStarted) {
			(*currTest)->test->Interrupted();
			HandleFinishedTest((*currTest)->test, true);
			currTest = tests.end();
		}
	}
}

void TestGroup::HandleFinishedTest(Test* test, bool testWasInterrupted) {
	std::vector<TestResult> results = test->GetResults();
	bool hasError = false;
	logger.Info("%s has finished", test->GetName().c_str());
	if (testWasInterrupted) {
		logger.Info("It was interrupted");
	}
	for (TestResult result : results) {
		this->results.push_back(result);
		if (result.GetType() == TestResult::Type::error) {
			hasError = true;
		}
	}
	if (testWasInterrupted) {
		if (hasError && stopOnError) {
			Interrupted();
		} else {
			test->End();
		}
	}
}

std::string TestGroup::GetName() {
	return name;
}
}
