/**
 * @file TestGroup.h
 * @author Lee Bousfield
 * @date Jan 31, 2015
 */

#ifndef TESTGROUP_H_
#define TESTGROUP_H_

#include "Test.h"
#include <vector>
#include <memory>

namespace tator {

class TestGroup: public Test {
	friend class Tester;
public:
	/**
	 * Creates a TestGroup
	 * @param name The name of the test group
	 * @param tests The list of tests
	 * @param stopOnError Should the test group stop on an error
	 */
	TestGroup(const char* name, std::vector<Test*> tests, bool stopOnError = true);

	/**
	 * Deletes the TestGroup and all child tests.
	 */
	~TestGroup();

	/**
	 * Clears the results of this test group and all children.
	 */
	void ClearResults() override;

	/**
	 * Adds a test to this TestGroup. Do not call after the tester has been started
	 * @param test The test to add.
	 */
	void AddTest(Test* test);

protected:
	/**
	 * Initializes the tests
	 */
	void Initialize() override;

	/**
	 * Executes the tests
	 */
	void Execute() override;

	/**
	 * Checks if all the tests are finished
	 */
	bool IsFinished() override;

	/**
	 * Ends all the tests
	 */
	void End() override;

	/**
	 * Interrupts all the tests
	 */
	void Interrupted() override;

	typedef std::vector<std::unique_ptr<Test>> Tests;
	Tests tests;
	Tests::iterator currentTest;
	bool stopOnError;
};
}

#endif /* TESTGROUP_H_ */
