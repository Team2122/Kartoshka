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
	rollers = new Talon(ports["rollers"].as<int>());
	finger = new DoubleSolenoid(ports["finger"].as<int>(),
			ports["finger"].as<int>());
	clampOne = new DoubleSolenoid(ports["clampOne"].as<int>(),
			ports["clampOne"].as<int>());
	clampTwo = new DoubleSolenoid(ports["clampTwo"].as<int>(),
			ports["clampTwo"].as<int>());
	button = new DigitalInput(ports["button"].as<int>());
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
		break;
	case kInward:
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
