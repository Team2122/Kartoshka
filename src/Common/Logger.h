/**
 * @file Logger.h
 * @author Nick Hodes
 * @date Jan 19, 2015
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <Timer.h>
#include <string>

namespace tator {

class USBManager;

/**
 * Is class for Logger. Give Logger name and will output message
 */
class Logger {
public:
	Logger(const char *name);

	/**
	 * Delete logger
	 */
	virtual ~Logger();

	/**
	 * Display logger level info
	 * @param const char *message for message of many words.
	 * @param anything else needed such as things needed for %c for char
	 */
	void Info(const char *message, ...)  __attribute__((format(printf, 2, 3)));

	/**
	 * Display logger level warn
	 * @param const char *message for message of many words.
	 * @param anything else needed such as things needed for %c for char
	 */
	void Warn(const char *message, ...);

	/**
	 * Display logger level error
	 * @param const char *message for message of many words.
	 * @param anything else needed such as things needed for %c for char
	 */
	void Error(const char *message, ...);

	/**
	 * Display logger level info
	 * @param message for message of many words
	 * @param vargs A va_list instead of a to be proccessed "..."
	 */
	void Info(const char *message, va_list vargs);

	/**
	 * Display logger level error
	 * @param message for message of many words
	 * @param vargs A va_list instead of a to be proccessed "..."
	 */
	void Error(const char *message, va_list vargs);

	/**
	 * Display logger level warn
	 * @param message for message of many words
	 * @param vargs A va_list instead of a to be proccessed "..."
	 */
	void Warn(const char *message, va_list vargs);

	const static int kMaxLength = 255;
private:
	/**
	 * Timestamp in seconds to 3 decimal places
	 */
	std::string MakeTimestamp();
	std::string name;
	USBManager* usb;

};

} /* namespace tator */

#endif /* LOGGER_H_ */
