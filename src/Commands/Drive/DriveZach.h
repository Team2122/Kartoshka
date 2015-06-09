/**
 * @file DriveZach.h
 * @author Alex Mikhalev
 * @date 5/30/2015
 */

#ifndef DRIVEZACH_H_
#define DRIVEZACH_H_

#include "CommandBase.h"

namespace tator {

class DriveZach: public CommandBase {
public:
	DriveZach(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(drive);
		kAngle = 0.02;
		kRate = 0.02;
		angleTolerance = .5;
		speed = config["speed"].as<double>();
		distance = config["distance"].as<double>();
		startAngle = config["startAngle"].as<double>();
		endAngle = config["endAngle"].as<double>();
		if (config["ramp"].IsDefined()) {
			rampDistance = config["ramp"]["distance"].as<double>();
			rampPower = config["ramp"]["power"].as<double>();
		} else {
			rampDistance = 0.0;
			rampPower = 0.0;
		}
		desiredRate = 0;
		startDistance = 0;
		currentDistance = 0;
		gyroAngle = 0;
	}

	static std::string GetBaseName() {
		return "DriveZach";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		startDistance = drive->GetDistance();
		double angleDelta = endAngle - startAngle;
		desiredRate = angleDelta / distance;
	}

	void Execute() override {
		currentDistance = fabs(startDistance - drive->GetDistance());
		double distanceLeft = distance - currentDistance;

		double rampedSpeed = speed;
		if (rampDistance != 0 && distanceLeft <= rampDistance) {
			double percentage = distanceLeft / rampDistance;
			rampedSpeed *= pow(percentage, rampPower);
		}

		double desiredAngle = desiredRate * currentDistance + startAngle;
		gyroAngle = otto->GetAngle();
		double angleDelta = desiredAngle - gyroAngle;

		double gyroRate = otto->GetRate();

		double offset = angleDelta * kAngle + desiredRate * kRate * rampedSpeed;
		log.Info(
				"gyroAngle: %f, desiredAngle: %f, gyroRate: %f, rate: %f, currentDistance: %f",
				gyroAngle, desiredAngle, gyroRate, desiredRate, currentDistance);
		drive->SetSpeeds(rampedSpeed + offset, rampedSpeed - offset);
	}

	bool IsFinished() override {
		double angleError = fabs(endAngle - gyroAngle);
		if (angleError <= angleTolerance) {
			log.Info("Angle delta within tolerance (%f <= %f). Finishing",
					angleError, angleTolerance);
			return true;
		}
		if (currentDistance >= distance) {
			log.Info("Reached distance (%f >= %f). Finishing", currentDistance,
					distance);
			return true;
		}
		return false;
	}

	void End() override {
		CommandBase::End();
		drive->SetSpeeds(0, 0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		drive->SetSpeeds(0, 0);
	}

private:
	double kAngle;
	double kRate;
	double angleTolerance;

	double speed;
	double distance;
	double startAngle;
	double endAngle;

	double rampDistance;
	double rampPower;

	double desiredRate;
	double startDistance;
	double currentDistance;
	double gyroAngle;
};

}

#endif /* DRIVEZACH_H_ */
