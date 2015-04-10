/**
 * @file DriveRotatePID.h
 * @author Alex Mikhalev
 */

#ifndef DRIVEROTATEPID_H
#define DRIVEROTATEPID_H

#include "CommandBase.h"
#include <PIDController.h>
#include "Subsystems/Drive.h"
#include "Subsystems/Otto.h"
#include <cmath>

namespace tator {

class DriveRotatePID: public CommandBase, public PIDSource, PIDOutput {
public:
	DriveRotatePID(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(drive);
		angle = config["angle"].as<float>();
		YAML::Node pid_ = config["PID"];
		pid = new PIDController(pid_["P"].as<float>(), pid_["I"].as<float>(),
				pid_["D"].as<float>(), pid_["F"].as<float>(), this, this);
		pid->SetAbsoluteTolerance(config["tolerance"].as<float>());
	}

	static std::string GetBaseName() {
		return "DriveRotatePID";
	}

protected:
	void PIDWrite(float output) override {
		drive->SetSpeeds(output, -output);
	}

	double PIDGet() override {
		return otto->GetAngle();
	}

	void Initialize() override {
		CommandBase::Initialize();
		const char* name;
		if (angle >= 0) {
			name = "clockwise";
		} else {
			name = "counterclockwise";
		}
		log.Info("Moving %s for %f degrees", name, angle);
		pid->Reset();
		pid->SetSetpoint(PIDGet() + angle);
		pid->Enable();
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return pid->OnTarget();
	}

	void End() override {
		CommandBase::End();
		log.Info("Finished rotating. target: %f, error: %f", angle,
				pid->GetError());
		pid->Disable();
		drive->SetSpeeds(0, 0);
	}

	void Interrupted() override {
		CommandBase::Interrupted();
		pid->Disable();
		drive->SetSpeeds(0, 0);
	}

private:
	PIDController* pid;
	float angle;
};

}

#endif
