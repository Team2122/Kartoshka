/*
 * Shuttle.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: Daniele
 */
#include "Shuttle.h"

namespace tator {

Shuttle::Shuttle(YAML::Node config) :
		SubsystemBase("Shuttle") {
	YAML::Node ports = config["Port"];
	toteSensor = new DigitalInput(ports["toteSensor"].as<int>());
	lowerSensor = new DigitalInput(ports["lowerSensor"].as<int>());
	upperSensor = new DigitalInput(ports["upperSensor"].as<int>());
	clampPiston = new DoubleSolenoid(
			config["Ports"]["clampPiston"][0].as<int>(),
			config["Ports"]["clampPiston"][1].as<int>());
	elevator = new Talon(config["elevator"].as<int>());
	totes = 0;
}

Shuttle::~Shuttle() {
}

bool Shuttle::IsTotePresent() {
	return toteSensor->Get();
}

Shuttle::Position Shuttle::ShuttlePosition() {
	if (lowerSensor->Get()) {
		return kLower;
	} else if (upperSensor->Get()) {
		return kUpper;
	} else {
		return kUnknown;
	}
}

void Shuttle::OpenProngs() {
	clampPiston->Set(DoubleSolenoid::kForward);
}

void Shuttle::CloseProngs() {
	clampPiston->Set(DoubleSolenoid::kReverse);
}

void Shuttle::SetShuttleSpeed(float speed) {
	elevator->SetSpeed(speed);
}

int Shuttle::GetToteCount() {
	return totes;
}

void Shuttle::IncrementToteCount(int increment) {
	totes = totes + increment;
}

void Shuttle::ResetToteCount() {
	totes = 0;
}

}
