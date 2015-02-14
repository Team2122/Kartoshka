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
	YAML::Node ports = config["Ports"];
	toteSensor = new DigitalInput(ports["toteSensor"].as<int>());
	lowerLimit = new DigitalInput(ports["lowerLimit"].as<int>());
	upperLimit = new DigitalInput(ports["upperLimit"].as<int>());
	clampPiston = new Solenoid(ports["clampPiston"].as<int>());
	YAML::Node lift = ports["lift"];
	liftMotor = new Talon(lift["motor"].as<int>());
	YAML::Node liftEncoder_ = lift["encoder"];
	liftEncoder = new Encoder(liftEncoder_[0].as<int>(),
			liftEncoder_[1].as<int>());
	pdp = new PowerDistributionPanel();
	motorPDPChannel = lift["pdp"].as<int>();
	fingersPiston = new Solenoid(ports["fingersPiston"].as<int>());

	YAML::Node values = config["Values"];
	upSpeed = values["upSpeed"].as<double>();
	downSpeed = values["downSpeed"].as<double>();
	holdSpeed = values["holdSpeed"].as<double>();
	maxMotorCurrent = values["maxCurrent"].as<double>();

	liftEncoder->SetReverseDirection(true);
	totes = 0;

	LiveWindow* liveWindow = LiveWindow::GetInstance();
	const char* name = GetName().c_str();
	liveWindow->AddSensor(name, "toteSensor", toteSensor);
	liveWindow->AddSensor(name, "lowerLimit", lowerLimit);
	liveWindow->AddSensor(name, "upperLimit", upperLimit);
	liveWindow->AddActuator(name, "clampPiston", clampPiston);
	liveWindow->AddActuator(name, "liftMotor", liftMotor);
	liveWindow->AddSensor(name, "liftEncoder", liftEncoder);
	liveWindow->AddSensor(name, "fingersPiston", fingersPiston);
}

Shuttle::~Shuttle() {
	delete toteSensor;
	delete lowerLimit;
	delete upperLimit;
	delete clampPiston;
	delete liftMotor;
	delete liftEncoder;
	delete fingersPiston;
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
	clampPiston->Set(true);
}

void Shuttle::CloseProngs() {
	clampPiston->Set(false);
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
		break;
	case kDown:
		liftMotor->SetSpeed(downSpeed);
		break;
	case kHold:
		liftMotor->SetSpeed(holdSpeed);
		break;
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

void Shuttle::SetFingersPiston(FingersState state) {
	if (state == kHeld) {
		fingersPiston->Set(true);
	} else if (state == kReleased) {
		fingersPiston->Set(false);
	} else {
		log.Error("Invalid fingers piston position");
	}

}

}
