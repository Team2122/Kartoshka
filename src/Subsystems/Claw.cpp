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
	upperLimit = new FixedField(ports["upperLimit"]["channel"].as<int>(),
			ports["upperLimit"]["isNPN"].as<bool>());
	homeLimit = new FixedField(ports["homeLimit"]["channel"].as<int>(),
			ports["homeLimit"]["isNPN"].as<bool>());

	clearClawRotate = soft["clearClaw"]["rotate"].as<double>();
	clearClawMinAngle = soft["clearClaw"]["minAngle"].as<double>();
	clearClawMinHeight = soft["clearClaw"]["minHeight"].as<double>();

	liftEncoder->SetDistancePerPulse(1.0 / 360);
	liftEncoder->SetPIDSourceParameter(PIDSource::kDistance);

	clampLong = new Solenoid(ports["clampLong"].as<int>());
	clampShort = new Solenoid(ports["clampShort"].as<int>());
	rollers = new Talon(ports["rollers"].as<int>());
	binSensor = new FixedField(ports["binSensor"]["channel"].as<int>(),
			ports["binSensor"]["isNPN"].as<bool>());

	rollerInwardSpeed = speed["inward"].as<double>();
	rollerOutwardSpeed = speed["outward"].as<double>();
	upSpeed = speed["up"].as<float>();
	downSpeed = speed["down"].as<float>();
	rotationFinished = false;
	clawAngle = ClawAngle::kPick;

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

void Claw::SetLiftSpeed(double power) {
	if (disabled) {
		return liftMotor->Set(0);
	}
	liftMotor->Set(power);
}

void Claw::SetLiftSpeed(LiftSpeed speed) {
	double clawAngle = rotationAngle->Get();
	double clawHeight = liftEncoder->GetDistance();
	if (disabled) {
		return liftMotor->Set(0);
	} else if (clawAngle > clearClawMinAngle && clawHeight < clearClawMinHeight
			&& false) {
		log.Error("Claw angle was %f > %f", clawAngle, clearClawMinAngle);
		return SetLiftSpeed(0);
	}
	SetLiftSpeed(GetLiftSpeed(speed));
	liftSpeed = speed;
}

Claw::LiftSpeed Claw::GetLiftSpeed() {
	return liftSpeed;
}

double Claw::GetLiftSpeed(LiftSpeed state) {
	switch (state) {
	case LiftSpeed::kUp:
		return (upSpeed);
	case LiftSpeed::kDown:
		return (downSpeed);
	default:
	case LiftSpeed::kStop:
		return (0);
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
	return homeLimit->Get();
}

bool Claw::IsTop() {
	return upperLimit->Get();
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
	return binSensor->Get();
}

void Claw::SetRotationSpeed(double speed, bool override) {
	double clawHeight = liftEncoder->GetDistance();
	double clawAngle = rotationAngle->Get();
	if (disabled && !override) {
		return clawRotation->SetSpeed(0);
	} else if (clawHeight < clearClawRotate && clawAngle > clearClawMinAngle
			&& speed != 0 && !override && false) {
		log.Error("Claw height was %f < %f, angle %f < %f", clawHeight,
				clearClawRotate, clawAngle, clearClawMinAngle);
		return clawRotation->SetSpeed(0);
	}
	clawRotation->SetSpeed(speed);
}

void Claw::SetRotationFinished(bool rotationFinished) {
	this->rotationFinished = rotationFinished;
}

bool Claw::IsRotationFinished() {
	return rotationFinished;
}

Claw::ClawAngle Claw::GetTargetAngle() {
	return clawAngle;
}

void Claw::SetTargetAngle(ClawAngle newClawAngle) {
	clawAngle = newClawAngle;
	SetRotationFinished(false);
}

}
