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
	liftVertical = new Talon(ports["liftVertical"].as<uint32_t>());
	clawRotation = new Talon(ports["liftRotation"].as<uint32_t>());
	auto verticalTicks_ = ports["verticalTicks"];
	verticalTicks = new Encoder(verticalTicks_[0].as<uint32_t>(),
			verticalTicks_[1].as<int>());
	auto rotationAngle_ = ports["rotationAngle"];
	rotationAngle = new AnalogPotentiometer(
			rotationAngle_["Channel"].as<uint32_t>(),
			rotationAngle_["Scale"].as<double>(),
			rotationAngle_["Offset"].as<double>());
	upperLimit = new DigitalInput(ports["upperLimit"].as<int>());

	maxLiftAngle = soft["liftAngle"]["max"].as<float>();
	minLiftAngle = soft["liftAngle"]["min"].as<float>();
	maxLiftHeight = soft["liftHeight"]["max"].as<float>();
	minLiftHeight = soft["liftHeight"]["min"].as<float>();
	clearClawMax = soft["clearClaw"]["max"].as<float>();
	clearClawMin = soft["clearClaw"]["min"].as<float>();

	verticalTicks->SetDistancePerPulse(1.0 / 360);
	verticalTicks->SetPIDSourceParameter(PIDSource::kDistance);

	clampOne = new Solenoid(ports["clampOne"].as<uint32_t>());
	clampTwo = new Solenoid(ports["clampTwo"].as<uint32_t>());
	rollers = new Talon(ports["rollers"].as<uint32_t>());
	binSensor = new DigitalInput(ports["binSensor"].as<uint32_t>());

	rollerInwardSpeed = speed["inward"].as<double>();
	rollerOutwardSpeed = speed["outward"].as<double>();
	forwardRotationSpeed = speed["forward"].as<float>();
	backwardRotationSpeed = speed["backward"].as<float>();

	LiveWindow* liveWindow = LiveWindow::GetInstance();
	auto name = GetName().c_str();
	liveWindow->AddActuator(name, "Rotation Motor", clawRotation);
	liveWindow->AddActuator(name, "Vertical Motor", liftVertical);
	liveWindow->AddSensor(name, "Rotation Encoder", rotationAngle);
	liveWindow->AddSensor(name, "Vertical Encoder", verticalTicks);
	liveWindow->AddSensor(name, "Upper Limit", upperLimit);
	liveWindow->AddActuator(name, "Clamp One", clampOne);
	liveWindow->AddActuator(name, "Clamp Two", clampTwo);
	liveWindow->AddSensor(name, "Button", binSensor);
	liveWindow->AddActuator(name, "Rollers", rollers);
}
Claw::~Claw() {
	delete liftVertical;
	delete clawRotation;
	delete verticalTicks;
	delete rotationAngle;

	delete rollers;
	delete clampOne;
	delete clampTwo;
	delete binSensor;
}

int32_t Claw::GetPosition() {
	return verticalTicks->Get();
}

void Claw::ResetTicks() {
	verticalTicks->Reset();
}

void Claw::SetRollerSpeed(RollerStatus status) {
	switch (status) {
	case RollerStatus::kOutward:
		rollers->Set(rollerOutwardSpeed);
		break;
	case RollerStatus::kInward:
		rollers->Set(rollerInwardSpeed);
		break;
	case RollerStatus::kStopped:
	default:
		rollers->Set(0.0);
		break;
	}

}

void Claw::SetClampStatus(ClampStatus status) {
	switch (status) {
	case ClampStatus::kLong:
		clampOne->Set(true);
		clampTwo->Set(true);
		break;
	case ClampStatus::kMid:
		clampOne->Set(true);
		clampTwo->Set(false);
		break;
	case ClampStatus::kShort:
		clampOne->Set(false);
		clampTwo->Set(true);
		break;
	case ClampStatus::kNone:
	default:
		clampOne->Set(false);
		clampTwo->Set(false);
		break;
	}
}

Claw::ClampStatus Claw::GetClampStatus() {
	return ClampStatus::kNone;
}

float Claw::GetRotationAngle() {
	return rotationAngle->Get();
}

bool Claw::HasContainer() {
	return binSensor->Get();
}

void Claw::SetRotationSpeed(RotationSpeed speed) {
	switch (speed) {
	case RotationSpeed::kForward:
		clawRotation->SetSpeed(forwardRotationSpeed);
		break;
	case RotationSpeed::kBackward:
		clawRotation->SetSpeed(backwardRotationSpeed);
		break;
	case RotationSpeed::kStopped:
	default:
		clawRotation->SetSpeed(0);
		break;
	}
}

}
