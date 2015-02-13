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

	clampLong = new Solenoid(ports["clampLong"].as<uint32_t>());
	clampShort = new Solenoid(ports["clampShort"].as<uint32_t>());
	rollers = new Talon(ports["rollers"].as<uint32_t>());
	binSensor = new DigitalInput(ports["binSensor"].as<uint32_t>());

	rollerInwardSpeed = speed["inward"].as<double>();
	rollerOutwardSpeed = speed["outward"].as<double>();
	forwardRotationSpeed = speed["forward"].as<float>();
	backwardRotationSpeed = speed["backward"].as<float>();

	clawClampStatus = 0;

	LiveWindow* liveWindow = LiveWindow::GetInstance();
	auto name = GetName().c_str();
	liveWindow->AddActuator(name, "Rotation Motor", clawRotation);
	liveWindow->AddActuator(name, "Vertical Motor", liftVertical);
	liveWindow->AddSensor(name, "Rotation Encoder", rotationAngle);
	liveWindow->AddSensor(name, "Vertical Encoder", verticalTicks);
	liveWindow->AddSensor(name, "Upper Limit", upperLimit);
	liveWindow->AddActuator(name, "Clamp Long", clampLong);
	liveWindow->AddActuator(name, "Clamp Short", clampShort);
	liveWindow->AddSensor(name, "Button", binSensor);
	liveWindow->AddActuator(name, "Rollers", rollers);
}
Claw::~Claw() {
	delete liftVertical;
	delete clawRotation;
	delete verticalTicks;
	delete rotationAngle;

	delete rollers;
	delete clampLong;
	delete clampShort;
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
	case ClampStatus::kDeathGrip:
		clampLong->Set(true);
		clampShort->Set(true);
		clawClampStatus = 3;
		break;
	case ClampStatus::kContainer:
		clampLong->Set(true);
		clampShort->Set(false);
		clawClampStatus = 2;
		break;
	case ClampStatus::kTote:
		clampLong->Set(false);
		clampShort->Set(true);
		clawClampStatus = 1;
		break;
	case ClampStatus::kReleased:
	default:
		clampLong->Set(false);
		clampShort->Set(false);
		clawClampStatus = 0;
		break;
	}
}

Claw::ClampStatus Claw::GetClampStatus() {
	switch (clawClampStatus) {
	case 3:
		return ClampStatus::kDeathGrip;
		break;
	case 2:
		return ClampStatus::kContainer;
		break;
	case 1:
		return ClampStatus::kTote;
		break;
	case 0:
		return ClampStatus::kReleased;
		break;
	default:
		return ClampStatus::kReleased;
		break;

	}
}

void Claw::ReleaseClaw() {
	switch (GetClampStatus()) {
	case ClampStatus::kDeathGrip:
		SetClampStatus(ClampStatus::kContainer);
		SetClampStatus(ClampStatus::kTote);
		SetClampStatus(ClampStatus::kReleased);
		break;
	case ClampStatus::kContainer:
		SetClampStatus(ClampStatus::kTote);
		SetClampStatus(ClampStatus::kReleased);
		break;
	case ClampStatus::kTote:
		SetClampStatus(ClampStatus::kReleased);
		break;
	case ClampStatus::kReleased:
		break;
	default:
		break;
	}
}

void Claw::ClampClaw() {
	switch (GetClampStatus()) {
	case ClampStatus::kDeathGrip:
		break;
	case ClampStatus::kContainer:
		SetClampStatus(ClampStatus::kDeathGrip);
		break;
	case ClampStatus::kTote:
		SetClampStatus(ClampStatus::kContainer);
		SetClampStatus(ClampStatus::kDeathGrip);
		break;
	case ClampStatus::kReleased:
		SetClampStatus(ClampStatus::kTote);
		SetClampStatus(ClampStatus::kContainer);
		SetClampStatus(ClampStatus::kDeathGrip);
		break;
	default:
		break;
	}
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
