/**
 * @file TestGroup.cpp
 * @author Lee Bousfield
 * @date Jan 31, 2015
 */

#include "TestGroup.h"
#include "TestResult.h"
#include "Tester.h"
#include <algorithm>

namespace tator {

TestGroup::TestGroup(const char* name, std::vector<Test*> tests,
		bool stopOnError) :
		Test(name), stopOnError(stopOnError) {
	for (Test* test : tests) {
		AddTest(test);
	}
	this->stopOnError = stopOnError;
}

TestGroup::~TestGroup() {
}

void TestGroup::Initialize() {
	currentTest = tests.begin();
	if (currentTest != tests.end()) { // if we have any tests
		(*currentTest)->Initialize();
	}
}

void TestGroup::ClearResults() {
	Test::ClearResults();
	std::for_each(tests.begin(), tests.end(), [](auto& test) {
		test->ClearResults();
	});
}

void TestGroup::AddTest(Test* test) {
	tests.push_back(std::unique_ptr<Test>(test));
	currentTest = tests.end();
}

void TestGroup::Execute() {
	if (currentTest != tests.end()) { // if we are not finished with that
		auto& test = *currentTest;
		test->Execute(); // run the test
		if (test->IsFinished()) { // if it has finished
			// end the test and get all results
			test->End();
			std::vector<TestResult> results = test->GetResults();
			// insert all of the results of the test to this test group
			this->results.insert(this->results.end(), results.begin(),
					results.end());
			// check if any results is an error
			bool hasError = std::any_of(results.begin(), results.end(),
					[](auto result) {
						return result.GetType() == TestResult::Type::error;
					});
			currentTest++; // and move on
			if (hasError && stopOnError) {
				Interrupted(); // interrupt self if there is an error and we should stop on errors
			}
			if (currentTest != tests.end()) { // unless we're at the last test
				(*currentTest)->Initialize(); // initialize it
			}
		}
	}
}

bool TestGroup::IsFinished() {
	return currentTest == tests.end(); // we're done when the iterator is at the end
}

void TestGroup::End() {
}

void TestGroup::Interrupted() {
	// if we havn't finished running sequentially
	if (currentTest != tests.end()) {
		// interrupt and mark as finished
		(*currentTest)->Interrupted();
		currentTest = tests.end();
	}
}

}
