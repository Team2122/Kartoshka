/**
 * @file ToteFeed.cpp
 * @author Lee Bousfield
 * @date Jan 24, 2015
 */

#include "ToteFeed.h"

namespace tator {

ToteFeed::ToteFeed(YAML::Node config) :
		SubsystemBase("ToteFeed") {
	YAML::Node ports = config["ports"];
	rollers = new Talon(ports["rollers"].as<int>());
	backSensor = new DigitalInput(ports["backSensor"].as<int>());
	rollerPiston = new Solenoid(ports["rollerPiston"].as<int>());
	flappers = new Talon(ports["flappers"].as<int>());
	flapperSpeed = config["flapperSpeed"].as<double>();
	LiveWindow* liveWindow = LiveWindow::GetInstance();
	liveWindow->AddActuator("ToteFeed", "Rollers", rollers);
	liveWindow->AddSensor("ToteFeed", "Back Sensor", backSensor);
	liveWindow->AddActuator("ToteFeed", "Roller Piston", rollerPiston);
	liveWindow->AddActuator("ToteFeed", "Flappers", flappers);
}

ToteFeed::~ToteFeed() {
	delete rollers;
	delete backSensor;
	delete rollerPiston;
}

bool ToteFeed::GetBackSensor() {
	return !backSensor->Get();
}

void ToteFeed::SetRollers(double speed) {
	rollers->SetSpeed(speed);
}

void ToteFeed::SetRollerPiston(RollerPistonState state) {
	rollerPiston->Set(state == RollerPistonState::up);
}

bool ToteFeed::GetRollerPiston() {
	return rollerPiston->Get();
}

void ToteFeed::SetFlapperSpeed(float speed) {
	flappers->SetSpeed(speed);
}

}
