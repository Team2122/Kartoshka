/**
 * The .cpp file for the Lift
 * @file Claw.cpp
 * @author Paul Vaughan
 */
#include "Claw.h"

namespace tator {

Claw::Claw(YAML::Node config) :
		SubsystemBase("Claw") {
	disabled = false;
	YAML::Node ports = config["Ports"];
	YAML::Node speed = config["Speeds"];
	YAML::Node soft = config["Software"];
	liftVertical = new Talon(ports["liftVertical"].as<int>());
	clawRotation = new Talon(ports["liftRotation"].as<int>());
	YAML::Node verticalTicks_ = ports["verticalTicks"];
	verticalTicks = new Encoder(verticalTicks_[0].as<int>(),
			verticalTicks_[1].as<int>());
	YAML::Node rotationAngle_ = ports["rotationAngle"];
	rotationAngle = new AnalogPotentiometer(rotationAngle_["Channel"].as<int>(),
			rotationAngle_["Scale"].as<double>(),
			rotationAngle_["Offset"].as<double>());
	upperLimit = new DigitalInput(ports["upperLimit"].as<int>());
	homeLimit = new DigitalInput(ports["homeLimit"].as<int>());

	clearClawRotate = soft["clearClaw"]["rotate"].as<double>();
	clearClawDescend = soft["clearClaw"]["descend"].as<double>();

	verticalTicks->SetDistancePerPulse(1.0 / 360);
	verticalTicks->SetPIDSourceParameter(PIDSource::kDistance);

	clampLong = new Solenoid(ports["clampLong"].as<int>());
	clampShort = new Solenoid(ports["clampShort"].as<int>());
	rollers = new Talon(ports["rollers"].as<int>());
	binSensor = new DigitalInput(ports["binSensor"].as<int>());

	rollerInwardSpeed = speed["inward"].as<double>();
	rollerOutwardSpeed = speed["outward"].as<double>();
	forwardRotationSpeed = speed["forward"].as<float>();
	backwardRotationSpeed = speed["backward"].as<float>();
	upSpeed = speed["up"].as<float>();
	downSpeed = speed["down"].as<float>();

	LiveWindow* liveWindow = LiveWindow::GetInstance();
	const char* name = GetName().c_str();
	liveWindow->AddActuator(name, "Rotation Motor", clawRotation);
	liveWindow->AddActuator(name, "Vertical Motor", liftVertical);
	liveWindow->AddSensor(name, "Rotation Encoder", rotationAngle);
	liveWindow->AddSensor(name, "Vertical Encoder", verticalTicks);
	liveWindow->AddSensor(name, "Home Limit", homeLimit);
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

void Claw::DisableClaw() {
	disabled = true;
}

void Claw::ReenableClaw() {
	disabled = false;
}

void Claw::SetVerticalLiftMotor(double power) {
	if (disabled) {
		return liftVertical->Set(0);
	}
	liftVertical->Set(power);
}

void Claw::SetLiftSpeed(LiftSpeed speed) {
	double clawAngle = rotationAngle->Get();
	if (clawAngle > clearClawDescend && false) {
		log.Error("Claw angle was %f > %f", clawAngle, clearClawDescend);
		return SetVerticalLiftMotor(0);
	}
	switch (speed) {
	case LiftSpeed::kUp:
		return SetVerticalLiftMotor(upSpeed);
	case LiftSpeed::kDown:
		return SetVerticalLiftMotor(downSpeed);
	default:
	case LiftSpeed::kStop:
		return SetVerticalLiftMotor(0);
	}
}

double Claw::GetLiftEncoder() {
	return verticalTicks->GetDistance();
}

void Claw::ZeroLiftEncoder() {
	verticalTicks->Reset();
}

int32_t Claw::GetPosition() {
	return verticalTicks->Get();
}

bool Claw::IsHome() {
	return !homeLimit->Get();
}

bool Claw::IsTop() {
	return !upperLimit->Get();
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
		clampLong->Set(false);
		clampShort->Set(false);
		break;
	case ClampStatus::kContainer:
		clampLong->Set(false);
		clampShort->Set(true);
		break;
	case ClampStatus::kTote:
		clampLong->Set(true);
		clampShort->Set(false);
		break;
	case ClampStatus::kReleased:
	default:
		clampLong->Set(true);
		clampShort->Set(true);
		break;
	}
}

float Claw::GetRotationAngle() {
	return rotationAngle->Get();
}

bool Claw::HasContainer() {
	return !binSensor->Get();
}

void Claw::SetRotationSpeed(RotationSpeed speed, bool override) {
	double clawHeight = verticalTicks->GetDistance();
	if (disabled && !override) {
		return clawRotation->SetSpeed(0);
	} else if (clawHeight < clearClawRotate && !override) {
		log.Error("Claw height was %f < %f", clawHeight, clearClawRotate);
		return clawRotation->SetSpeed(0);
	}
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
