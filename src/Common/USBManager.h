/**
 * @file USBManager.h
 * @author Alex Mikhalev
 * @date Mar 18, 2015
 */

#ifndef USBMANAGER_H_
#define USBMANAGER_H_

#include "Logger.h"
#include <string>
#include <cstdio>

namespace tator {

class USBManager {
public:
	constexpr const static char* kLogDirectory = "/media/sda1/logs/";

	USBManager();
	virtual ~USBManager();

	static USBManager* GetInstance();

	void Initialize();

	void Fprintf(const char* fmt, ...);
	void Flush();

private:
	static USBManager* instance;
	static Logger* log; // :p
	FILE* file;

};

} /* namespace tator */

#endif /* USBMANAGER_H_ */
