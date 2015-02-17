/**
 * The .cpp file for the Lift
 * @file Claw.cpp
 * @author Paul Vaughan
 */
#include "Claw.h"
#include "Common/ManualTester.h"

namespace tator {

Claw::Claw(YAML::Node config) :
		SubsystemBase("Claw") {
	disabled = false;
	YAML::Node ports = config["Ports"];
	YAML::Node speed = config["Speeds"];
	YAML::Node soft = config["Software"];
	liftMotor = new Talon(ports["liftVertical"].as<int>());
	clawRotation = new Talon(ports["liftRotation"].as<int>());
	YAML::Node verticalTicks_ = ports["verticalTicks"];
	liftEncoder = new Encoder(verticalTicks_[0].as<int>(),
			verticalTicks_[1].as<int>());
	YAML::Node rotationAngle_ = ports["rotationAngle"];
	rotationAngle = new AnalogPotentiometer(rotationAngle_["Channel"].as<int>(),
			rotationAngle_["Scale"].as<double>(),
			rotationAngle_["Offset"].as<double>());
	upperLimit = new DigitalInput(ports["upperLimit"].as<int>());
	homeLimit = new DigitalInput(ports["homeLimit"].as<int>());

	clearClawRotate = soft["clearClaw"]["rotate"].as<double>();
	clearClawMinAngle = soft["clearClaw"]["minAngle"].as<double>();
	clearClawMinHeight = soft["clearClaw"]["minHeight"].as<double>();

	liftEncoder->SetDistancePerPulse(1.0 / 360);
	liftEncoder->SetPIDSourceParameter(PIDSource::kDistance);

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

	ManualTester* manualTester = ManualTester::GetInstance();
	std::string name = GetName();
	manualTester->Add(name, "claw rotation", clawRotation, 0.3);
	manualTester->Add(name, "claw lift", liftMotor);
	manualTester->Add(name, "claw rotation", rotationAngle);
	manualTester->Add(name, "claw lift", liftEncoder);
	manualTester->Add(name, "home limit", homeLimit);
	manualTester->Add(name, "upper limit", upperLimit);
	manualTester->Add(name, "long clamp", clampLong);
	manualTester->Add(name, "short clamp", clampShort);
	manualTester->Add(name, "bin sensor", binSensor);
	manualTester->Add(name, "claw rollers", rollers);
	currentSpeed = RotationSpeed::kUnknown;
	targetAngle = 0;
}
Claw::~Claw() {
	delete liftMotor;
	delete clawRotation;
	delete liftEncoder;
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
		return liftMotor->Set(0);
	}
	liftMotor->Set(power);
}

void Claw::SetLiftSpeed(LiftSpeed speed) {
	double clawAngle = rotationAngle->Get();
	double clawHeight = liftEncoder->GetDistance();
	if (clawAngle > clearClawMinAngle && clawHeight < clearClawMinHeight) {
		log.Error("Claw angle was %f > %f", clawAngle, clearClawMinAngle);
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
	return liftEncoder->GetDistance();
}

void Claw::ZeroLiftEncoder() {
	liftEncoder->Reset();
}

int32_t Claw::GetPosition() {
	return liftEncoder->Get();
}

bool Claw::IsHome() {
	return !homeLimit->Get();
}

bool Claw::IsTop() {
	return !upperLimit->Get();
}

void Claw::ResetTicks() {
	liftEncoder->Reset();
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
	double clawHeight = liftEncoder->GetDistance();
	if (disabled && !override) {
		return clawRotation->SetSpeed(0);
	} else if (clawHeight < clearClawRotate && speed != RotationSpeed::kStopped
			&& speed != RotationSpeed::kUnknown && !override) {
		log.Error("Claw height was %f < %f", clawHeight, clearClawRotate);
		return clawRotation->SetSpeed(0);
	}
	currentSpeed = speed;
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

Claw::RotationSpeed Claw::GetRotationSpeed() {
	return currentSpeed;

}

double Claw::GetTargetAngle() {
	return targetAngle;
}

void Claw::SetTargetAngle(double newTargetAngle) {
	targetAngle = newTargetAngle;
	if (GetRotationSpeed() == RotationSpeed::kStopped)
		SetRotationSpeed(RotationSpeed::kUnknown);
}

}
