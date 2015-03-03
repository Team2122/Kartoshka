/**
 * @file Tester.h
 * @author Lee Bousfield
 * @date Feb 3, 2015
 */

#include "TestGroup.h"

#ifndef TESTER_H_
#define TESTER_H_

namespace tator {
class Tester: public TestGroup {
public:
	/**
	 * Gets the instance of the Tester
	 * Creates it if it does not exist
	 * @return The instance of the Tester
	 */
	static Tester* GetInstance();

	/**
	 * Called when the tester ends
	 */
	void End();

private:
	/**
	 * Initializes the Tester
	 */
	Tester();

	/**
	 * Destrory the Tester
	 */
	~Tester();

	/// The instance of the tester
	static Tester* instance;
	/// All the tests
	std::vector<Test*> allTests;
};
}

#endif /* TESTER_H_ */
