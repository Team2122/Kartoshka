/**
 * @file TestResult.cpp
 * @date Jan 29, 2015
 */

#include "TestResult.h"

namespace tator {
TestResult::TestResult(Logger* logger, Type type, const char* message) {
	this->logger = logger;
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
		logger->Info(message);
		break;
	case Type::warning:
		logger->Warn(message);
		break;
	case Type::error:
		logger->Error(message);
		break;
	}
}
}
