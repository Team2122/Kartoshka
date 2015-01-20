/**
 * @file Logger.h
 * @author Nick Hodes
 * @date Jan 19, 2015
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

namespace tator {

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
	void Info(const char *message, ...);

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

private:
	std::string name;
	const int kMaxLength = 255;

};

} /* namespace tator */

#endif /* LOGGER_H_ */
