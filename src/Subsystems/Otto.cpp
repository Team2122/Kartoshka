/**
 * The .cpp file for Otto
 * @file Otto.cpp
 * @author Nick Hodes
 */
#include "Otto.h"

namespace tator {

Otto::Otto(YAML::Node config) :
		SubsystemBase("Otto") {
	YAML::Node ports = config["Ports"];
	YAML::Node autoSwitch = ports["switch"];
	autoSwitch1 = new DigitalInput(autoSwitch[0].as<int>());
	autoSwitch2 = new DigitalInput(autoSwitch[1].as<int>());
	autoSwitch3 = new DigitalInput(autoSwitch[2].as<int>());
	LiveWindow *liveWindow = LiveWindow::GetInstance();
	liveWindow->AddSensor(GetName().c_str(), "Auto Switch One", autoSwitch1);
	liveWindow->AddSensor(GetName().c_str(), "Auto Switch Two", autoSwitch2);
	liveWindow->AddSensor(GetName().c_str(), "Auto Switch Three", autoSwitch3);
}

Otto::~Otto() {
}

int Otto::GetAutoModeNumber(){

}

} /* namespace tator */
