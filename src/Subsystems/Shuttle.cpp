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
	auto ports = config["Ports"];
	toteSensor = new DigitalInput(ports["toteSensor"].as<uint32_t>());
	lowerLimit = new DigitalInput(ports["lowerLimit"].as<uint32_t>());
	upperLimit = new DigitalInput(ports["upperLimit"].as<uint32_t>());
	auto clampPiston_ = ports["clampPiston"];
	clampPiston = new DoubleSolenoid(clampPiston_[0].as<uint32_t>(),
			clampPiston_[1].as<uint32_t>());
	auto lift = ports["lift"];
	liftMotor = new Talon(lift["motor"].as<uint32_t>());
	auto liftEncoder_ = lift["encoder"];
	liftEncoder = new Encoder(liftEncoder_[0].as<uint32_t>(),
			liftEncoder_[1].as<uint32_t>());
	pdp = new PowerDistributionPanel();
	motorPDPChannel = lift["pdp"].as<uint32_t>();

	auto values = config["Values"];
	upSpeed = values["upSpeed"].as<double>();
	downSpeed = values["downSpeed"].as<double>();
	maxMotorCurrent = values["maxCurrent"].as<double>();

	liftEncoder->SetReverseDirection(true);
	totes = 0;

	auto liveWindow = LiveWindow::GetInstance();
	auto name = GetName().c_str();
	liveWindow->AddSensor(name, "toteSensor", toteSensor);
	liveWindow->AddSensor(name, "lowerLimit", lowerLimit);
	liveWindow->AddSensor(name, "upperLimit", upperLimit);
	liveWindow->AddActuator(name, "clampPiston", clampPiston);
	liveWindow->AddActuator(name, "liftMotor", liftMotor);
	liveWindow->AddSensor(name, "liftEncoder", liftEncoder);
}

Shuttle::~Shuttle() {
	delete toteSensor;
	delete lowerLimit;
	delete upperLimit;
	delete clampPiston;
	delete liftMotor;
	delete liftEncoder;
}

bool Shuttle::IsTotePresent() {
	return !toteSensor->Get();
}

Shuttle::Position Shuttle::GetLimit() {
	if (!lowerLimit->Get()) { // the banner fixed-fields are inverted
		return kLower;
	} else if (!upperLimit->Get()) {
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

int Shuttle::GetToteCount() {
	return totes;
}

void Shuttle::IncrementToteCount(int increment) {
	totes = totes + increment;
}

void Shuttle::SetShuttleSpeed(Speed speed) {
	switch (speed) {
	case kUp:
		liftMotor->SetSpeed(upSpeed);
	case kDown:
		liftMotor->SetSpeed(downSpeed);
	case kStop:
	default:
		liftMotor->SetSpeed(0);
		break;
	}
}

void Shuttle::ResetToteCount() {
	totes = 0;
}

int32_t Shuttle::GetEncoderTicks() {
	return liftEncoder->Get();
}

bool Shuttle::IsStalled() {
	return pdp->GetCurrent(motorPDPChannel) >= maxMotorCurrent;
}

void Shuttle::ResetEncoder() {
	liftEncoder->Reset();
}

}
