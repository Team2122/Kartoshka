/**
 * @file SuccessfulTest.h
 * @author Lee Bousfield
 * @date Feb 4, 2015
 */

#ifndef SUCCESSFULTEST_H_
#define SUCCESSFULTEST_H_

#include "Common/Tester/Test.h"

namespace tator {

class SuccessfulTest: public Test {
public:
	SuccessfulTest() :
			Test("SuccessfulTest") {
		untilDone = 0;
	}

protected:
	void Initialize() override {
		Test::Initialize();
		untilDone = 5;
	}

	void Execute() override {
		log.Info("Execute calling, will execute %d more times", untilDone);
		Success("Executed (%d more to go)", untilDone);
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

#endif /* SUCCESSFULTEST_H_ */
