/**
 * @file TestResult.cpp
 * @date Jan 29, 2015
 */

#include "TestResult.h"

namespace tator {
TestResult::TestResult(Logger* log, Type type, const char* message) {
	this->log = log;
	this->type = type;
	this->message = message;
}

TestResult::~TestResult() {
}

TestResult::Type TestResult::GetType() {
	return type;
}

void TestResult::Log() {
	switch (type) {
	case Type::success:
	case Type::info:
		log->Info(message);
		break;
	case Type::warning:
		log->Warn(message);
		break;
	case Type::error:
		log->Error(message);
		break;
	}
}
}
