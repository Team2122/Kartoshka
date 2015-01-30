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
	char buffer[kMaxLength];
	va_list args;
	va_start(args, message);
	vsnprintf(buffer, kMaxLength, message, args);
	va_end(args);
	std::cout << "[INFO] " << name << " - " << buffer << std::endl;
}

void Logger::Warn(const char *message, ...) {
	char buffer[kMaxLength];
	va_list args;
	va_start(args, message);
	vsnprintf(buffer, kMaxLength, message, args);
	va_end(args);
	std::cout << "[WARN] " << name << " - " << buffer << std::endl;
}

void Logger::Error(const char *message, ...) {
	char buffer[kMaxLength];
	va_list args;
	va_start(args, message);
	vsnprintf(buffer, kMaxLength, message, args);
	va_end(args);
	std::cerr << "[ERROR] " << name << " - " << buffer << std::endl;
}

void Logger::Info(const char *message, va_list vargs) {
	char buffer[kMaxLength];
	vsnprintf(buffer, kMaxLength, message, vargs);
	std::cout << "[INFO] " << name << " - " << buffer << std::endl;
}

void Logger::Warn(const char *message, va_list vargs) {
	char buffer[kMaxLength];
	vsnprintf(buffer, kMaxLength, message, vargs);
	std::cout << "[WARN] " << name << " - " << buffer << std::endl;
}

void Logger::Error(const char *message, va_list vargs) {
	char buffer[kMaxLength];
	vsnprintf(buffer, kMaxLength, message, vargs);
	std::cerr << "[ERROR] " << name << " - " << buffer << std::endl;
}

} /* namespace tator */
