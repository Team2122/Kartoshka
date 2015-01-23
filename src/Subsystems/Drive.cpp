/**
 * @file Drive.cpp
 * @author Daniele Moro
 * @date Jan 21, 2015
 */

#include "Drive.h"

namespace tator {

Drive::Drive(YAML::Node& config) :
		SubsystemBase("Drive") {
	driveL = new Talon(config["drive"]["L"].as<int>());
	driveR = new Talon(config["drive"]["R"].as<int>());
	YAML::Node conEncL = config["encoder"]["L"];
	YAML::Node conEncR = config["encoder"]["R"];
	encoderL = new Encoder(conEncL[0].as<int>(), conEncL[1].as<int>());
	encoderR = new Encoder(conEncR[0].as<int>(), conEncR[1].as<int>());
	YAML::Node pidl = config["PID"]["L"];
	YAML::Node pidr = config["PID]"]["R"];
	pidL = new PIDController(pidl["P"].as<uint32_t>(), pidl["I"].as<uint32_t>(),
			pidl["D"].as<uint32_t>(), pidl["F"].as<uint32_t>(), encoderL,
			driveL);
	pidR = new PIDController(pidr["P"].as<uint32_t>(), pidr["I"].as<uint32_t>(),
			pidr["D"].as<uint32_t>(), pidr["F"].as<uint32_t>(), encoderR,
			driveR);

	encoderL->SetDistancePerPulse(1 / 360);
	encoderR->SetDistancePerPulse(1 / 360);
	encoderL->SetPIDSourceParameter(PIDSource::kRate);
	encoderR->SetPIDSourceParameter(PIDSource::kRate);
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
	pidL->SetSetpoint(leftSpeed);
	pidR->SetSetpoint(rightSpeed);
}

} /* namespace tator */

