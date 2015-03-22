/**
 * @file USBManager.cpp
 * @author Alex Mikhalev
 * @date Mar 18, 2015
 */

#include "Common/USBManager.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

namespace tator {

USBManager* USBManager::instance = nullptr;
Logger* USBManager::log = nullptr;

USBManager::USBManager() {
	file = nullptr;
}

USBManager::~USBManager() {
}

USBManager* USBManager::GetInstance() {
	if (instance == nullptr) {
		instance = new USBManager();
	}
	return instance;
}

void USBManager::Initialize() {
	if (log == nullptr)
		log = new Logger("USBManager");
	log->Info("Opening log directory...");
	DIR* dir;
	if ((dir = opendir(kLogDirectory)) == nullptr) {
		if (errno == ENOENT) {
			if (!mkdir(kLogDirectory, 0755)) {
				return log->Error("Creating log directory: %s", strerror(errno));
			} else {
				log->Info("Created log directory");
			}
		} else {
			return log->Error("Opening log directory: %s", strerror(errno));
		}
	} else {
		closedir(dir);
	}
	time_t rawtime;
	struct tm* timeinfo;
	char buf[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, sizeof(buf), "log %C %a %b %d %H.%M.%S.txt", timeinfo);
	char fname[120];
	strcpy(fname, kLogDirectory);
	strcat(fname, buf);
	log->Info("Opening log file: \"%s\"", fname);
	file = fopen(fname, "w");
	if (file == nullptr) {
		return log->Error("Opening log file: %s", strerror(errno));
	}
	log->Info("Done");
}

void USBManager::Fprintf(const char* fmt, ...) {
	if (file == nullptr)
		return;
	va_list varargs;
	va_start(varargs, fmt);
	vfprintf(file, fmt, varargs);
	va_end(varargs);
}

void USBManager::Flush() {
	if (file == nullptr)
		return;
	fflush(file);
	sync();
}

} /* namespace tator */
