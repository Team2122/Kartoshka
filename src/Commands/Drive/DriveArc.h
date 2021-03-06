/**
 * @file DriveArc.h
 * @author Alex Mikhalev
 * @date 3/21/2015
 */

#ifndef DRIVEARC_H_
#define DRIVEARC_H_

#include "DriveStraight.h"

namespace tator {

class DriveArc: public DriveStraight {
public:
	DriveArc(std::string name, YAML::Node config) :
			DriveStraight(name, config) {
		degreesPerFoot = config["degreesPerFoot"].as<double>();
	}

	static std::string GetBaseName() {
		return "DriveArc";
	}

protected:
	void Initialize() override {
		DriveStraight::Initialize();
	}

	void Execute() override {
		DriveStraight::Execute();
		pid->SetSetpoint(currentDistance * degreesPerFoot);
	}

private:
	double degreesPerFoot;
};

}

#endif /* DRIVEARC_H_ */
