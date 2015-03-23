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
		targetAngle = 0;
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
		double distance = drive->GetDistance() - startDistance;
		targetAngle = fabs(distance) * degreesPerFoot;
		log.Info("distance: %f, targetAngle: %f", distance, targetAngle);
		DriveStraight::Execute();
	}

private:
	double degreesPerFoot;
};

}

#endif /* DRIVEARC_H_ */
