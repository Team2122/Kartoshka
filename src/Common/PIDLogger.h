/*
 * PIDLogger.h
 *
 *  Created on: Jun 20, 2015
 *      Author: lee
 */

#ifndef PIDLOGGER_H_
#define PIDLOGGER_H_

#include <PIDOutput.h>
#include <PIDSource.h>
#include <cstdio>
#include <string>
#include "Logger.h"

namespace tator {

class PIDLogger: public PIDSource, public PIDOutput {
public:
	PIDLogger(PIDSource* in, PIDOutput* out, std::string name);
	virtual ~PIDLogger();
	virtual double PIDGet() override;
	virtual void PIDWrite(float output) override;
private:
	PIDSource* in;
	PIDOutput* out;
	constexpr const static char* kLogDirectory = "/media/sda1/PIDlogs/";
	Logger* log;
	FILE* file;
};

}

#endif /* PIDLOGGER_H_ */
