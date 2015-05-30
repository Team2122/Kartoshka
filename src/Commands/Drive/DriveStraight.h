/**
 * @file DriveStraight.h
 * @author Alex Mikhalev
 * @date 3/21/2015
 */

#ifndef DRIVESTRAIGHT_H_
#define DRIVESTRAIGHT_H_

#include "CommandBase.h"
#include "Subsystems/Otto.h"
#include "Subsystems/Drive.h"
#include <PIDSource.h>
#include <PIDOutput.h>

namespace tator {

class DriveStraight: public CommandBase, public PIDSource, public PIDOutput {
public:
	DriveStraight(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(drive);
		currentDistance = 0;
		startDistance = 0;
		angle = config["angle"].as<double>();
		speed = config["speed"].as<double>();
		distance = config["distance"].as<double>();
		YAML::Node pid_ = config["PID"];
		pid = new PIDController(pid_["P"].as<double>(), pid_["I"].as<double>(),
				pid_["D"].as<double>(), pid_["F"].as<double>(), this, this);
	}

	static std::string GetBaseName() {
		return "DriveStraight";
	}

protected:
	double PIDGet() override {
		double angle = otto->GetAngle();
		return angle;
	}

	void PIDWrite(float output) override {
		drive->SetSpeeds(speed + output, speed - output);
	}

	double currentDistance;
	PIDController* pid;

	void Initialize() override {
		CommandBase::Initialize();
		startDistance = drive->GetDistance();
		pid->SetSetpoint(angle);
		pid->Reset();
		pid->Enable();
	}

	void Execute() override {
		currentDistance = fabs(drive->GetDistance() - startDistance);
		log.Info("angle: %f, target: %f, distance: %f", PIDGet(), pid->GetSetpoint(), currentDistance);
	}

	bool IsFinished() override {
		return currentDistance >= distance;
	}

	void End() override {
		CommandBase::End();
		pid->Disable();
		drive->SetSpeeds(0, 0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		pid->Disable();
		drive->SetSpeeds(0, 0);
	}

private:
	double distance, speed, angle;
	double startDistance;
};

}

#endif /* DRIVESTRAIGHT_H_ */
