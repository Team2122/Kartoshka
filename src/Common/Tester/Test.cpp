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
		log(name.c_str()), name(name) {
}

Test::~Test() {
}

std::string Test::GetName() {
	return name;
}

std::vector<TestResult> Test::GetResults() {
	return results;
}

void Test::ClearResults() {
	results.clear();
}

void Test::Initialize() {
	log.Info("Test %s has initialized", GetName().c_str());
}

void Test::End() {
	log.Info("Test %s has ended", GetName().c_str());
}

void Test::Interrupted() {
	log.Info("Test %s has been interrupted", GetName().c_str());
}

void Test::AddTestResult(TestResult::Type resultType, const char* message,
		va_list vargs) {
	char* buffer = new char[Logger::kMaxLength];
	vsnprintf(buffer, Logger::kMaxLength, message, vargs);
	results.push_back(TestResult(&log, resultType, buffer));
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
