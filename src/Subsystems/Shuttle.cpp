/*
 * Shuttle.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: Daniele
 */
#include "Shuttle.h"
#include "Common/Tester/ManualTester.h"
#include "CommandBase.h" // For accessing the level 1 tote sensor from ToteFeed
#include "Subsystems/ToteFeed.h"

namespace tator {

Shuttle::Shuttle(YAML::Node config) :
		SubsystemBase("Shuttle") {
	YAML::Node ports = config["Ports"];
	toteSensor = new FixedField(ports["toteSensor"]["channel"].as<int>(),
			ports["toteSensor"]["isNPN"].as<bool>());
	lowerLimit = new FixedField(ports["lowerLimit"]["channel"].as<int>(),
			ports["lowerLimit"]["isNPN"].as<bool>());
	upperLimit = new FixedField(ports["upperLimit"]["channel"].as<int>(),
			ports["upperLimit"]["isNPN"].as<bool>());
	clampPiston = new Solenoid(ports["clampPiston"].as<int>());
	YAML::Node lift = ports["lift"];
	liftMotor = new Talon(lift["motor"].as<int>());
	YAML::Node liftEncoder_ = lift["encoder"];
	liftEncoder = new Encoder(liftEncoder_[0].as<int>(),
			liftEncoder_[1].as<int>());
	fingersPiston = new Solenoid(ports["fingersPiston"].as<int>());

	YAML::Node values = config["Values"];
	upSpeeds = values["upSpeeds"].as<std::vector<double>>();
	if (upSpeeds.size() != kNumUpSpeeds) {
		log.Error("%d up speeds were specified instead of %d", upSpeeds.size(),
				kNumUpSpeeds);
		exit(-1);
	}
	downSpeed = values["downSpeed"].as<double>();
	holdSpeed = values["holdSpeed"].as<double>();

	liftEncoder->SetReverseDirection(true);

	toteHeight = 6;
	totesHeld = 0;
	totesRatcheted = 0;

	ManualTester* manualTester = ManualTester::GetInstance();
	std::string name = GetName();
	manualTester->Add(name, "tote sensor", toteSensor);
	manualTester->Add(name, "lower limit", lowerLimit);
	manualTester->Add(name, "upper limit", upperLimit);
	manualTester->Add(name, "shuttle clamp", clampPiston);
	manualTester->Add(name, "shuttle lift", liftMotor, 0.4);
	manualTester->Add(name, "shuttle lift", liftEncoder);
	manualTester->Add(name, "shuttle fingers", fingersPiston);
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

bool Shuttle::HasToteAtShuttleBase() {
	return toteSensor->Get();
}

Shuttle::Position Shuttle::GetLimit() {
	if (lowerLimit->Get()) {
		return kLower;
	} else if (upperLimit->Get()) {
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

int Shuttle::GetDesiredTotes() {
	return toteHeight;
}

void Shuttle::SetDesiredTotes(int height) {
	toteHeight = height;
}

int Shuttle::GetTotesHeld() {
	return totesHeld;
}

void Shuttle::SetTotesHeld(int count) {
	totesHeld = count;
}

int Shuttle::GetTotesRatcheted() {
	return totesRatcheted;
}

void Shuttle::ZeroTotesRatcheted() {
	totesRatcheted = 0;
}

void Shuttle::UpdateTotesRatcheted() {
	totesRatcheted = totesHeld;
	if (CommandBase::toteFeed->GetBackSensor()) {
		totesRatcheted--; // That tote isn't ratcheted
	}
	if (HasToteAtShuttleBase()) {
		totesRatcheted--;
	}
	log.Info("UpdateTotesRatcheted: Desired Totes: %d, Totes Held: %d, "
			"Totes Ratcheted: %d", GetDesiredTotes(), GetTotesHeld(),
			GetTotesRatcheted());
}

void Shuttle::SetShuttleSpeed(Speed state) {
	SetShuttleSpeed(GetShuttleSpeed(state));
}

double Shuttle::GetShuttleSpeed(Speed state) {
	switch (state) {
	case kUp:
		if (totesRatcheted >= kNumUpSpeeds) {
			return upSpeeds[kNumUpSpeeds - 1];
		} else {
			return upSpeeds[totesRatcheted];
		}
	case kDown:
		return downSpeed;
	case kHold:
		return holdSpeed;
	case kStop:
	default:
		return 0.0;
	}
}

void Shuttle::SetShuttleSpeed(double speed) {
	liftMotor->SetSpeed(speed);
}

int32_t Shuttle::GetEncoderTicks() {
	return liftEncoder->Get();
}

void Shuttle::ResetEncoder() {
	liftEncoder->Reset();
}

void Shuttle::SetFingersPiston(FingersState state) {
	if (state == kHeld) {
		fingersPiston->Set(false);
	} else if (state == kReleased) {
		fingersPiston->Set(true);
	} else {
		log.Error("Invalid fingers piston position");
	}

}

}
