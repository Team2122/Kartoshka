/**
 * @file Tester.h
 * @author Lee Bousfield
 * @date Feb 3, 2015
 */

#ifndef TESTER_H_
#define TESTER_H_

#include "TestGroup.h"
#include "ManualTester.h"
#include "Joystick.h"
#include <memory>

namespace tator {
class Tester {
public:
	/**
	 * Gets the instance of the Tester
	 * Creates it if it does not exist
	 * @return The instance of the Tester
	 */
	static Tester* GetInstance();

	/**
	 * Creates all of the tests for the tester
	 */
	void CreateTests();

	/**
	 * Initializes the tester. Should be called once before run.
	 */
	void Initialize();

	/**
	 * Runs the tester. Should be called periodically during test.
	 */
	void Run();

	/**
	 * Disables the tester. Should be called once during disabled.
	 */
	void Disable();

	/**
	 * Prints the results of automated testing
	 */
	void PrintResults();

	/**
	 * Adds a test to the tester. Do not call after starting the tester.
	 * @param test The test to add
	 */
	void AddTest(Test* test);

private:
	/**
	 * Initializes the Tester
	 */
	Tester();

	/**
	 * Destroys the Tester
	 */
	~Tester();

	/// The instance of the tester
	static Tester* instance;
	Logger log;
	Joystick* joy;
	ManualTester* manualTester;
	std::unique_ptr<TestGroup> tests; ///< All the tests

	enum class TestMode {
		disabled, autonomous, manual, finished
	};

	TestMode testMode;
};
}

#endif /* TESTER_H_ */
