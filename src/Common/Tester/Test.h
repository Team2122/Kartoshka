/**
 * @file Test.h
 * @author Lee Bousfield
 * @date Jan 27, 2015
 */

#ifndef TEST_H_
#define TEST_H_

#include <string>
#include <vector>
#include "TestResult.h"

namespace tator {

class Test {
	friend class Tester;
	friend class TestGroup;
public:
	/**
	 * Constructor for Test
	 * @param name The name of the test
	 */
	Test(std::string name);

	/**
	 * Deletes Test
	 */
	virtual ~Test();

	/**
	 * Gets the name of the test
	 * @return the name of the test
	 */
	std::string GetName();

	/**
	 * Gets the list of results for this test
	 * @return The list of results for this test
	 */
	std::vector<TestResult> GetResults();

	/**
	 * Clears the list of results
	 */
	virtual void ClearResults();

protected:
	/**
	 * Called when a test is ready to be executed
	 */
	virtual void Initialize();

	/**
	 * Called periodically so a test can test
	 */
	virtual void Execute() = 0;

	/**
	 * Checks if Test IsFinished
	 */
	virtual bool IsFinished() = 0;

	/**
	 * Called when a test ends
	 */
	virtual void End();

	/**
	 * Called when a test is interrupted
	 * End will not be called
	 */
	virtual void Interrupted();

	/**
	 * Adds a test result to the list of results
	 * @param resultType The type of the result
	 * @param message The message of the result
	 * @param vargs The virtual arguments passed to the logger
	 */
	void AddTestResult(TestResult::Type resultType, const char* message,
			va_list vargs);

	/**
	 * Adds an error to the list of results
	 * @param message The message to be passed to the logger
	 * @param ... The arguments to be passed to the logger
	 */
	void Success(const char* message, ...);
	/**
	 * Adds an error to the list of results
	 * @param message The message to be passed to the logger
	 * @param ... The arguments to be passed to the logger
	 */
	void Warn(const char* message, ...);
	/**
	 * Adds an error to the list of results
	 * @param message The message to be passed to the logger
	 * @param ... The arguments to be passed to the logger
	 */
	void Info(const char* message, ...);
	/**
	 * Adds an error to the list of results
	 * @param message The message to be passed to the logger
	 * @param ... The arguments to be passed to the logger
	 */
	void Error(const char* message, ...);

	/// The logger for this test
	Logger log;

private:
	/// The list of "results" that occurred (for more info see the TestResult class)
	std::vector<TestResult> results;

	/// The name of the test
	std::string name;
};

}

#endif /* TESH_H_ */
