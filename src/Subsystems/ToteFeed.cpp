/**
 * @file ToteFeed.cpp
 * @author Lee Bousfield
 * @date Jan 24, 2015
 */

#include "ToteFeed.h"
#include "Common/ManualTester.h"

namespace tator {

ToteFeed::ToteFeed(YAML::Node config) :
		SubsystemBase("ToteFeed") {
	YAML::Node ports = config["ports"];
	rollers = new Talon(ports["rollers"].as<int>());
	backSensor = new FixedField(ports["backSensor"]["channel"].as<int>(),
			ports["backSensor"]["isNPN"].as<bool>());
	flappers = new Talon(ports["flappers"].as<int>());
	ManualTester* manualTester = ManualTester::GetInstance();
	std::string name = GetName();
	manualTester->Add(name, "rollerbed", rollers);
	manualTester->Add(name, "bottom tote sensor", backSensor);
	manualTester->Add(name, "intake rollers (flappers)", flappers);
}

ToteFeed::~ToteFeed() {
	delete rollers;
	delete backSensor;
}

bool ToteFeed::GetBackSensor() {
	return backSensor->Get();
}

void ToteFeed::SetRollers(double speed) {
	rollers->SetSpeed(speed);
}

void ToteFeed::SetFlapperSpeed(float speed) {
	flappers->SetSpeed(speed);
}

}
