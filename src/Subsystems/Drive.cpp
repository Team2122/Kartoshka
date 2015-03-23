/**
 * @file Drive.cpp
 * @author Daniele Moro
 * @date Jan 21, 2015
 */

#include "Drive.h"
#include "Common/ManualTester.h"

namespace tator {

Drive::Drive(YAML::Node config) :
		SubsystemBase("Drive") {
	driveL = new Talon(config["drive"]["L"].as<int>());
	driveR = new Talon(config["drive"]["R"].as<int>());
	YAML::Node conEncL = config["encoder"]["L"];
	YAML::Node conEncR = config["encoder"]["R"];
	encoderL = new Encoder(conEncL[0].as<int>(), conEncL[1].as<int>());
	encoderR = new Encoder(conEncR[0].as<int>(), conEncR[1].as<int>());
	YAML::Node pidl = config["PID"]["L"];
	YAML::Node pidr = config["PID"]["R"];
	pidL = new PIDController(pidl["P"].as<double>(), pidl["I"].as<double>(),
			pidl["D"].as<double>(), pidl["F"].as<double>(), encoderL, driveL);
	pidR = new PIDController(pidr["P"].as<double>(), pidr["I"].as<double>(),
			pidr["D"].as<double>(), pidr["F"].as<double>(), encoderR, driveR);
	maxRPS = config["maxRPS"].as<double>();

	encoderL->SetDistancePerPulse(1.0 / 360.0);
	encoderR->SetDistancePerPulse(1.0 / 360.0);
	encoderL->SetPIDSourceParameter(PIDSource::kRate);
	encoderR->SetPIDSourceParameter(PIDSource::kRate);
	pidL->Enable();
	pidR->Enable();

	ManualTester* manualTester = ManualTester::GetInstance();
	manualTester->Add(GetName(), "left drive", driveL);
	manualTester->Add(GetName(), "right drive", driveR);
	manualTester->Add(GetName(), "left drive", encoderL);
	manualTester->Add(GetName(), "right drive", encoderR);
	manualTester->Add(GetName(), "left drive", pidL, maxRPS);
	manualTester->Add(GetName(), "right drive", pidR, maxRPS);
}

Drive::~Drive() {
	delete driveL;
	delete driveR;
	delete encoderL;
	delete encoderR;
	delete pidL;
	delete pidR;
}

void Drive::SetSpeeds(float leftSpeed, float rightSpeed) {
	leftSpeed *= maxRPS;
	rightSpeed *= maxRPS;
	SetRPS(leftSpeed, rightSpeed);
}

double Drive::GetDistance() {
	double distance = ((-encoderL->GetDistance()) + encoderR->GetDistance())
			/ 2;
	return (12.5663 * (distance / 12)); // 12 inches per feet
}

void Drive::SetRPS(float leftRPS, float rightRPS) {
	pidL->SetSetpoint(-leftRPS);
	pidR->SetSetpoint(rightRPS);
}

} /* namespace tator */
