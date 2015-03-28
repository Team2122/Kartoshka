/**
 * The .cpp for the Thief
 * @file Thief.cpp
 * @author Nick Hodes
 */

#include "Thief.h"
#include "Common/Tester/ManualTester.h"

namespace tator {

Thief::Thief(YAML::Node config) :
		SubsystemBase("Thief") {
	piston = new Solenoid(config["port"].as<int>());

	ManualTester* manualTester = ManualTester::GetInstance();
	std::string name = GetName();
	manualTester->Add(name, "thief piston", piston);
}

Thief::~Thief() {
	delete piston;
}

void Thief::Set(State state){
	switch (state) {
	case State::steal:
		piston->Set(true);
		break;
	case State::nosteal:
		piston->Set(false);
		break;
	default:
		piston->Set(false);
		break;
	}
}
}
