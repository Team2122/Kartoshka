/*
 * PIDLogger.cpp
 *
 *  Created on: Jun 20, 2015
 *      Author: lee
 */

#include <Common/PIDLogger.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

namespace tator {

PIDLogger::PIDLogger(PIDSource* in, PIDOutput* out, std::string name) {
	// Stolen from USBManager
	if (log == nullptr)
		log = new Logger("PIDLogger");
	log->Info("Opening PID log directory...");
	DIR* dir;
	if ((dir = opendir(kLogDirectory)) == nullptr) {
		if (errno == ENOENT) {
			if (!mkdir(kLogDirectory, 0755)) {
				log->Error("Creating PID log directory: %s", strerror(errno));
				return;
			} else {
				log->Info("Created PID log directory");
			}
		} else {
			log->Error("Opening PID log directory: %s", strerror(errno));
			return;
		}
	} else {
		closedir(dir);
	}
	this->in = in;
	this->out = out;
	time_t rawtime;
	struct tm* timeinfo;
	char buf[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, sizeof(buf), ("PIDlog_" + name + " %C %a %b %d %H.%M.%S.txt").c_str(), timeinfo);
	char fname[120];
	strcpy(fname, kLogDirectory);
	strcat(fname, buf);
	log->Info("Opening log file: \"%s\"", fname);
	file = fopen(fname, "w");
	if (file == nullptr) {
		log->Error("Opening log file: %s", strerror(errno));
		return;
	}
}

PIDLogger::~PIDLogger() {
}


double PIDLogger::PIDGet() {
	double get = in->PIDGet();
	fprintf(file, "g%f\n", get);
	return get;
}

void PIDLogger::PIDWrite(float output) {
	fprintf(file, "g%f\n", output);
	return out->PIDWrite(output);
}

}
