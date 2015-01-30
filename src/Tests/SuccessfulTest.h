/**
 * @file SuccessfulTest.h
 * @author Lee Bousfield
 * @date Feb 4, 2015
 */

#include "Common/Test.h"

#ifndef SUCCESSFULTEST_H_
#define SUCCESSFULTEST_H_

namespace tator {
class SuccessfulTest: public Test {
public:

	SuccessfulTest() :
			Test("SuccessfulTest") {
		untilDone = 0;
	}

	void Initialize() {
		logger.Info("Initialize called");
		untilDone = 5;
	}

	void Execute() {
		logger.Info("Execute calling, will execute %d more times", untilDone);
		Success("Executed (%d more to go)", untilDone);
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

#endif /* SUCCESSFULTEST_H_ */
