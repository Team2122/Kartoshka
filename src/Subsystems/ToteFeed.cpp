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
	LiveWindow* liveWindow = LiveWindow::GetInstance();
	liveWindow->AddActuator("ToteFeed", "Rollers", rollers);
	liveWindow->AddSensor("ToteFeed", "Back Sensor", backSensor);
	liveWindow->AddActuator("ToteFeed", "Roller Piston", rollerPiston);
}

ToteFeed::~ToteFeed() {
	delete rollers;
	delete backSensor;
	delete rollerPiston;
}

bool ToteFeed::GetBackSensor() {
	return backSensor->Get();
}

void ToteFeed::SetRollers(double speed) {
	rollers->Set(speed);
}

void ToteFeed::SetRollerPiston(RollerPistonState state) {
	rollerPiston->Set(state == RollerPistonState::up);
}

bool ToteFeed::GetRollerPiston() {
	return rollerPiston->Get();
}

}
