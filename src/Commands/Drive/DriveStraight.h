/**
 * @file DriveStraight.h
 * @author Alex Mikhalev
 * @date 3/21/2015
 */

#ifndef DRIVESTRAIGHT_H_
#define DRIVESTRAIGHT_H_

#include "Robot.h"
#include "Subsystems/Otto.h"
#include "Subsystems/Drive.h"
#include <PIDSource.h>
#include <PIDOutput.h>

namespace tator {

class DriveStraight: public RobotCommand, public PIDSource, public PIDOutput {
public:
	DriveStraight(std::string name, YAML::Node config) :
			RobotCommand(name) {
		Requires(robot->drive);
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
		double angle = robot->otto->GetAngle();
		return angle;
	}

	void PIDWrite(float output) override {
		robot->drive->SetSpeeds(speed + output, speed - output);
	}

	double currentDistance;
	PIDController* pid;

	void Initialize() override {
		RobotCommand::Initialize();
		startDistance = robot->drive->GetDistance();
		pid->SetSetpoint(angle);
		pid->Reset();
		pid->Enable();
	}

	void Execute() override {
		currentDistance = fabs(robot->drive->GetDistance() - startDistance);
		log.Info("angle: %f, target: %f, distance: %f", PIDGet(),
				pid->GetSetpoint(), currentDistance);
	}

	bool IsFinished() override {
		return currentDistance >= distance;
	}

	void End() override {
		RobotCommand::End();
		pid->Disable();
		robot->drive->SetSpeeds(0, 0);
	}

	void Interrupted() override {
		RobotCommand::Interrupted();
		pid->Disable();
		robot->drive->SetSpeeds(0, 0);
	}

private:
	double distance, speed, angle;
	double startDistance;
};

}

#endif /* DRIVESTRAIGHT_H_ */
