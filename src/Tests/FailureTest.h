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
	FailureTest() :
			Test("FailureTest") {
		untilDone = 0;
	}

protected:
	void Initialize() override {
		Test::Initialize();
		untilDone = 5;
	}

	void Execute() override {
		log.Info("Execute calling, will execute %d more times", untilDone);
		Error("Executed (%d more to go)", untilDone);
		untilDone--;
	}

	bool IsFinished() override {
		log.Info("IsFinished called");
		return untilDone < 0;
	}

private:
	int untilDone;
};

}

#endif /* FAILURETEST_H_ */
