/**
 * The .cpp file for Otto
 * @file Otto.cpp
 * @author Nick Hodes
 */
#include "Otto.h"
#include "Common/ManualTester.h"

namespace tator {

Otto::Otto(YAML::Node config) :
		SubsystemBase("Otto") {
	YAML::Node ports = config["Ports"];
	YAML::Node autoSwitch = ports["switch"];
	autoSwitch1 = new DigitalInput(autoSwitch[0].as<int>());
	autoSwitch2 = new DigitalInput(autoSwitch[1].as<int>());
	autoSwitch3 = new DigitalInput(autoSwitch[2].as<int>());
	ManualTester* manualTester = ManualTester::GetInstance();
	manualTester->Add(GetName(), "auto switch one", autoSwitch1);
	manualTester->Add(GetName(), "auto switch two", autoSwitch2);
	manualTester->Add(GetName(), "auto switch three", autoSwitch3);
}

Otto::~Otto() {
}

int Otto::GetAutoModeNumber(){

}

} /* namespace tator */
