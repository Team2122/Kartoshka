/**
 * @file TestGroup.h
 * @author Lee Bousfield
 * @date Jan 31, 2015
 */

#ifndef TESTGROUP_H_
#define TESTGROUP_H_

#include "Test.h"

namespace tator {

struct TestData {
	/// The test
	Test* test;
	/// Is the test finished?
	bool isFinished;
	/// Has the test started?
	bool hasStarted;
};

class TestGroup: public Test {
public:

	/**
	 * Creates a TestGroup
	 * @param name The name of the test group
	 * @param tests The list of tests
	 * @param synchronous Should these tests run one at a time
	 * @param stopOnError Should the test group stop on an error
	 */
	TestGroup(const char* name, std::vector<Test*> tests, bool simultaneous,
			bool stopOnError = true);

	/**
	 * Deletes the TestGroup
	 */
	~TestGroup();

	/**
	 * Initializes the tests
	 */
	void Initialize();

	/**
	 * Executes the tests
	 */
	void Execute();

	/**
	 * Checks if all the tests are finished
	 */
	bool IsFinished();

	/**
	 * Ends all the tests
	 */
	void End();

	/**
	 * Interrupts all the tests
	 */
	void Interrupted();

	/**
	 * Returns the name of the TestGroup
	 */
	std::string GetName();
protected:

	/**
	 * Handles a finished test
	 * @param test The test
	 * @param testWasInterrupted If this test was just interrupted
	 */
	void HandleFinishedTest(Test* test, bool testWasInterrupted = false);

	std::vector<TestData*> tests;
	std::vector<TestData*>::iterator currTest;
	std::string name;
	bool simultaneous;
	bool stopOnError;
};
}

#endif /* TESTGROUP_H_ */
