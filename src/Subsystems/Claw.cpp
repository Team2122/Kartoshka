/**
 * The .cpp file for the Lift
 * @file Claw.cpp
 * @author Paul Vaughan
 */

#include "Claw.h"
#include "Common/Tester/ManualTester.h"

namespace tator {

Claw::Claw(YAML::Node config) :
		SubsystemBase("Claw") {
	enabled = true;

	YAML::Node ports = config["Ports"];
	YAML::Node soft = config["Software"];

	YAML::Node lift = ports["lift"];
	liftMotor = new Talon(lift["motor"].as<int>());
	YAML::Node liftEncoder = lift["encoder"];
	this->liftEncoder = new Encoder(liftEncoder[0].as<int>(),
			liftEncoder[1].as<int>());
	this->liftEncoder->SetDistancePerPulse(1.0 / 360);
	this->liftEncoder->SetPIDSourceParameter(PIDSource::kDistance);
	heightTolerance = soft["heightTolerance"].as<double>();

	YAML::Node limits = ports["limits"];
	YAML::Node homeLimit = limits["home"];
	this->homeLimit = new FixedField(homeLimit["channel"].as<int>(),
			homeLimit["isNPN"].as<bool>());
	YAML::Node topLimit = limits["top"];
	this->topLimit = new FixedField(topLimit["channel"].as<int>(),
			topLimit["isNPN"].as<bool>());

	YAML::Node rotation = ports["rotation"];
	rotationMotor = new Talon(rotation["motor"].as<int>());
	YAML::Node rotationEncoder = rotation["encoder"];
	this->rotationEncoder = new AnalogPotentiometer(
			rotationEncoder["Channel"].as<int>(),
			rotationEncoder["Scale"].as<double>(),
			rotationEncoder["Offset"].as<double>());
	YAML::Node angles = soft["angles"];
	angleTolerance = angles["tolerance"].as<double>();
	frontAngle = angles["front"].as<double>();
	backAngle = angles["back"].as<double>();

	YAML::Node clamp = ports["clamp"];
	clampLeft = new Solenoid(clamp["left"].as<int>());
	clampRight = new Solenoid(clamp["right"].as<int>());

	rollers = new Talon(ports["rollers"].as<int>());

	YAML::Node binSensor = ports["binSensor"];
	this->binSensor = new FixedField(binSensor["channel"].as<int>(),
			binSensor["isNPN"].as<bool>());

	YAML::Node speeds = config["Speeds"];
	rollerInwardSpeed = speeds["inward"].as<double>();
	rollerOutwardSpeed = speeds["outward"].as<double>();

	ManualTester* manualTester = ManualTester::GetInstance();
	std::string name = GetName();
	manualTester->Add(name, "claw lift", liftMotor);
	manualTester->Add(name, "claw lift", this->liftEncoder);
	manualTester->Add(name, "claw rotation", rotationMotor, 0.3);
	manualTester->Add(name, "claw rotation", this->rotationEncoder);
	manualTester->Add(name, "home limit", this->homeLimit);
	manualTester->Add(name, "upper limit", this->topLimit);
	manualTester->Add(name, "left clamp", clampLeft);
	manualTester->Add(name, "right clamp", clampRight);
	manualTester->Add(name, "claw rollers", rollers);
	manualTester->Add(name, "bin sensor", this->binSensor);
}

Claw::~Claw() {
	delete liftMotor;
	delete liftEncoder;
	delete topLimit;
	delete homeLimit;
	delete rotationMotor;
	delete rotationEncoder;
	delete clampLeft;
	delete clampRight;
	delete rollers;
	delete binSensor;
}

void Claw::DisableClaw() {
	enabled = false;
}

void Claw::EnableClaw() {
	enabled = true;
}

void Claw::SetLiftSpeed(double power) {
	if (!enabled) {
		return liftMotor->Set(0);
	}
	liftMotor->Set(power);
}

double Claw::GetLiftEncoder() {
	return liftEncoder->GetDistance();
}

void Claw::ZeroLiftEncoder() {
	liftEncoder->Reset();
}

bool Claw::IsAtHeight(double height) {
	return fabs(height - GetLiftEncoder()) < heightTolerance;
}

bool Claw::IsAtHome() {
	return homeLimit->Get();
}

bool Claw::IsAtTop() {
	return topLimit->Get();
}

Claw::ClawAngle Claw::AngleFromName(std::string name) {
	if (name == "front") {
		return ClawAngle::front;
	} else if (name == "back") {
		return ClawAngle::back;
	} else if (name == "neither") {
		return ClawAngle::neither;
	} else {
		log.Error("%s isn't a valid claw angle name", name.c_str());
		throw std::runtime_error("invalid angle");
	}
}

bool Claw::IsAtAngle(ClawAngle angle) {
	double currentAngle = rotationEncoder->Get();
	switch (angle) {
	case ClawAngle::neither:
		return true;
	case ClawAngle::front:
		return fabs(frontAngle - currentAngle) <= angleTolerance;
	case ClawAngle::back:
		return fabs(backAngle - currentAngle) <= angleTolerance;
	default:
		return false;
	}
}

void Claw::SetRotationSpeed(double speed) {
	if (!enabled) {
		return rotationMotor->SetSpeed(0);
	}
	rotationMotor->SetSpeed(speed);
}

double Claw::GetDegreesFromAngle(ClawAngle angle) {
	double currentAngle = rotationEncoder->Get();
	switch (angle) {
	case ClawAngle::front:
		return fabs(frontAngle - currentAngle);
	case ClawAngle::back:
		return fabs(backAngle - currentAngle);
	default:
		return 0;
	}
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
		clampLeft->Set(false);
		clampRight->Set(false);
		break;
	case ClampStatus::kRight:
		clampLeft->Set(false);
		clampRight->Set(true);
		break;
	case ClampStatus::kLeft:
		clampLeft->Set(true);
		clampRight->Set(false);
		break;
	case ClampStatus::kReleased:
	default:
		clampLeft->Set(true);
		clampRight->Set(true);
		break;
	}
}

Claw::ClampStatus Claw::GetClampStatus() {
	bool isLeft = clampLeft->Get();
	bool isRight = clampRight->Get();
	if (isLeft) {
		if (isRight) {
			return ClampStatus::kReleased;
		} else {
			return ClampStatus::kLeft;
		}
	} else {
		if (isRight) {
			return ClampStatus::kRight;
		} else {
			return ClampStatus::kDeathGrip;
		}
	}
}

bool Claw::HasContainer() {
	return binSensor->Get();
}

}
