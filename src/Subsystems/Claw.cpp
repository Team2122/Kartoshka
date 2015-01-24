/**
 * The .cpp file for the Claw
 * @file Claw.cpp
 * @author Paul Vaughan
 */
#include "Claw.h"

namespace tator {

Claw::Claw(YAML::Node config) :
		SubsystemBase("Claw") {
	YAML::Node ports = config["Ports"];
	YAML::Node speed = config["Speeds"];
	clampOne = new DoubleSolenoid(ports["clampOne"][0].as<int>(),
			ports["clampOne"][1].as<int>());
	clampTwo = new DoubleSolenoid(ports["clampTwo"][0].as<int>(),
			ports["clampTwo"][1].as<int>());
	finger = new DoubleSolenoid(ports["finger"][0].as<int>(),
			ports["finger"][1].as<int>());
	rollers = new Talon(ports["rollers"].as<int>());
	button = new DigitalInput(ports["button"].as<int>());

	rollerInwardSpeed = speed["inward"].as<double>();
	rollerOutwardSpeed = speed["outward"].as<double>();
}
Claw::~Claw() {
	delete rollers;
	delete clampOne;
	delete clampTwo;
	delete finger;
	delete button;
}

void Claw::SetRollerSpeed(ClawRollerStatus operation) {
	switch (operation) {
	case kOutward:
		rollers->Set(rollerOutwardSpeed);
		break;
	case kInward:
		rollers->Set(rollerInwardSpeed);
		break;
	default:
		rollers->Set(0.0);
		break;
	}

}

void Claw::ClampContainer() {
	clampOne->Set(DoubleSolenoid::kForward);
	clampTwo->Set(DoubleSolenoid::kForward);
}

void Claw::ReleaseContainer() {
	clampOne->Set(DoubleSolenoid::kReverse);
	clampTwo->Set(DoubleSolenoid::kReverse);
}

bool Claw::HasContainer() {
	return button->Get();
}

}
