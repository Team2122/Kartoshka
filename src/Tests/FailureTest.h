/**
 * @file FailureTest.h
 * @author Lee Bousfield
 * @date Feb 4, 2015
 */

#ifndef FAILURETEST_H_
#define FAILURETEST_H_

#include "Common/Tester/Test.h"

namespace tator {
class FailureTest: public Test {
public:

	FailureTest() :
			Test("FailureTest") {
		untilDone = 0;
	}

	void Initialize() {
		logger.Info("Initialize called");
		untilDone = 5;
	}

	void Execute() {
		logger.Info("Execute calling, will execute %d more times", untilDone);
		Error("Executed (%d more to go)", untilDone);
		untilDone--;
	}

	void End() {
	}

	void Interrupted() {
	}

	bool IsFinished() {
		logger.Info("IsFinished called");
		return untilDone < 0;
	}

protected:
	int untilDone;
};
}

#endif /* FAILURETEST_H_ */
