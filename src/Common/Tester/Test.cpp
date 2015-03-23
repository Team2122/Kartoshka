/**
 * @file Test.cpp
 * @author Lee Bousfield
 * @date Jan 27, 2015
 */

#include "Test.h"
#include <cstdio>
#include <cstdarg>
#include <iostream>

namespace tator {
Test::Test(std::string name) :
		name(name), logger(name.c_str()) {
}

Test::~Test() {
}

std::string Test::GetName() {
	return name;
}

std::vector<TestResult> Test::GetResults() {
	return results;
}

void Test::AddTestResult(TestResult::Type resultType, const char* message,
		va_list vargs) {
	char* buffer = new char[Logger::kMaxLength];
	vsnprintf(buffer, Logger::kMaxLength, message, vargs);
	results.push_back(TestResult(&logger, resultType, buffer));
}

void Test::Success(const char* message, ...) {
	va_list vargs;
	va_start(vargs, message);
	AddTestResult(TestResult::Type::success, message, vargs);
	va_end(vargs);
}

void Test::Info(const char* message, ...) {
	va_list vargs;
	va_start(vargs, message);
	AddTestResult(TestResult::Type::info, message, vargs);
	va_end(vargs);
}

void Test::Warn(const char* message, ...) {
	va_list vargs;
	va_start(vargs, message);
	AddTestResult(TestResult::Type::warning, message, vargs);
	va_end(vargs);
}

void Test::Error(const char* message, ...) {
	va_list vargs;
	va_start(vargs, message);
	AddTestResult(TestResult::Type::error, message, vargs);
	va_end(vargs);
}

} /* namespace tator */
