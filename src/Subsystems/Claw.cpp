/**
 * The .cpp file for the Lift
 * @file Claw.cpp
 * @author Paul Vaughan
 */
#include "Claw.h"

namespace tator {

Claw::Claw(YAML::Node config) :
		SubsystemBase("Claw") {
	YAML::Node ports = config["Ports"];
	YAML::Node speed = config["Speeds"];
	YAML::Node soft = config["Software"];
	liftVertical = new Talon(ports["liftVertical"].as<int>());
	clawRotation = new Talon(ports["liftRotation"].as<int>());
	verticalTicks = new Encoder(ports["verticalTicks"][0].as<int>(),
			ports["verticalTicks"][1].as<int>());
	rotationAngle = new AnalogPotentiometer(
			ports["rotationAngle"]["Channel"].as<int>(),
			ports["rotationAngle"]["Scale"].as<double>(),
			ports["rotationAngle"]["Offset"].as<double>());

	minLiftAngle = soft["LiftAngle"]["min"].as<float>();
	maxLiftAngle = soft["LiftAngle"]["max"].as<float>();
	maxLiftHeight = soft["LiftHeight"]["max"].as<float>();
	minLiftHeight = soft["LiftHeight"]["min"].as<float>();
	clearClawMax = soft["clearClawMax"]["max"].as<float>();
	clearClawMin = soft["clearClawMin"]["min"].as<float>();

	verticalTicks->SetDistancePerPulse(1 / 360);
	verticalTicks->SetPIDSourceParameter(PIDSource::kDistance);

	clampOne = new DoubleSolenoid(ports["clampOne"][0].as<int>(),
			ports["clampOne"][1].as<int>());
	clampTwo = new DoubleSolenoid(ports["clampTwo"][0].as<int>(),
			ports["clampTwo"][1].as<int>());
	rollers = new Talon(ports["rollers"].as<int>());
	button = new DigitalInput(ports["button"].as<int>());

	rollerInwardSpeed = speed["inward"].as<double>();
	rollerOutwardSpeed = speed["outward"].as<double>();
	forwardRotationSpeed = speed["forward"].as<float>();
	backwardRotationSpeed = speed["backward"].as<float>();

	LiveWindow* liveWindow = LiveWindow::GetInstance();
	liveWindow->AddActuator(GetName().c_str(), "Rotation Motor", clawRotation);
	liveWindow->AddSensor(GetName().c_str(), "Rotation Encoder", rotationAngle);
}
Claw::~Claw() {
	delete liftVertical;
	delete clawRotation;
	delete verticalTicks;
	delete rotationAngle;

	delete rollers;
	delete clampOne;
	delete clampTwo;
	delete button;
}

/*
 * if bottom sensor is not triggered and angle of claw is not 0 or 180, do not do anything
 */

//Returns the current height of the robot
double Claw::GetPosition() {
	return verticalTicks->Get();
}

//Resets the encoder to the value of 0
void Claw::ResetTicks() {
	verticalTicks->Reset();
}

//Return the rotation angle of the lift
double Claw::GetAngle() {
	float encoderval = rotationAngle->Get();
	if (encoderval < minLiftAngle) {
		encoderval = minLiftAngle;
	} else if (encoderval > maxLiftAngle) {
		encoderval = maxLiftAngle;
	}
	return (encoderval - minLiftAngle) / maxLiftAngle * 360.0;
}

//Da roller speed is become in, out, or oof.
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

//Puny container is crushed from Mother Russia's technologicaly superior claw.
void Claw::SetContainerClampStatus(ClawClampStatus extendTo) {
	switch (extendTo) {
	case kLong:
		clampOne->Set(DoubleSolenoid::kForward);
		clampTwo->Set(DoubleSolenoid::kForward);
		break;
	case kMid:
		clampOne->Set(DoubleSolenoid::kForward);
		clampTwo->Set(DoubleSolenoid::kOff);
		break;
	case kShort:
		clampOne->Set(DoubleSolenoid::kOff);
		clampTwo->Set(DoubleSolenoid::kForward);
		break;
	case kNone:
		clampOne->Set(DoubleSolenoid::kOff);
		clampTwo->Set(DoubleSolenoid::kOff);
		break;
	default:
		clampOne->Set(DoubleSolenoid::kOff);
		clampTwo->Set(DoubleSolenoid::kOff);
		break;
	}
}

Claw::ClawClampStatus Claw::GetContainerClampStatus() {
	return kNone;
}

float Claw::GetRoatationAngle() {
	return rotationAngle->Get();
}

//The motherland demand robot to say if it has shown mercy to puny
bool Claw::HasContainer() {
	return button->Get();
}

void Claw::SetRotationDirection(RotationDirection dir) {
	switch (dir) {
	case kForward:
		clawRotation->SetSpeed(forwardRotationSpeed);
		break;
	case kBackward:
		clawRotation->SetSpeed(backwardRotationSpeed);
		break;
	default:
		clawRotation->SetSpeed(0);
		break;
	}
}

}
