/**
 * @file TestResult.h
 * @date Jan 29, 2015
 */

#ifndef TESTRESULT_H_
#define TESTRESULT_H_

#include "Logger.h"

namespace tator {
class TestResult {
public:
	enum class Type {
		success, info, warning, error
	};

	TestResult(Logger* logger, Type resultType, const char* message);
	~TestResult();

	Type GetType();
	void Log();
protected:
	Logger* logger;
	Type type;
	const char* message;
};
}

#endif /* TESTRESULT_H_ */
