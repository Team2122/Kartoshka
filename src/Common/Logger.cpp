/**
 * @file Logger.cpp
 * @author Nick Hodes
 * @date Jan 19, 2015
 */

#include "Logger.h"
#include <cstdio>
#include <cstdarg>
#include <iostream>

namespace tator {
Logger::Logger(const char *name) :
		name(name) {

}

Logger::~Logger() {

}

void Logger::Info(const char *message, ...) {
	va_list args;
	va_start(args, message);
	Info(message, args);
	va_end(args);
}

void Logger::Warn(const char *message, ...) {
	va_list args;
	va_start(args, message);
	Warn(message, args);
	va_end(args);
}

void Logger::Error(const char *message, ...) {
	va_list args;
	va_start(args, message);
	Error(message, args);
	va_end(args);
}

void Logger::Info(const char *message, va_list vargs) {
	char buffer[kMaxLength];
	vsnprintf(buffer, kMaxLength, message, vargs);
	std::cout << MakeTimestamp() << " [INFO] " << name << " - " << buffer
			<< std::endl;
}

void Logger::Warn(const char *message, va_list vargs) {
	char buffer[kMaxLength];
	vsnprintf(buffer, kMaxLength, message, vargs);
	std::cout << MakeTimestamp() << " [WARN] " << name << " - " << buffer
			<< std::endl;
}

void Logger::Error(const char *message, va_list vargs) {
	char buffer[kMaxLength];
	vsnprintf(buffer, kMaxLength, message, vargs);
	std::cerr << MakeTimestamp() << " [ERROR] " << name << " - " << buffer
			<< std::endl;
}

std::string Logger::MakeTimestamp() {
	char buffer[kMaxLength];
	snprintf(buffer, kMaxLength, "%8.3f", Timer::GetFPGATimestamp());
	return std::string(buffer);
}

} /* namespace tator */
