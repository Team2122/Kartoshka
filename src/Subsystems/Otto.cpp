/**
 * The .cpp file for Otto
 * @file Otto.cpp
 * @author Nick Hodes
 */
#include <CameraServer.h>
#include "Otto.h"
#include "Common/Tester/ManualTester.h"
#include "Common/Config/Kremlin.h"

namespace tator {

Otto::Otto(YAML::Node config) :
		SubsystemBase("Otto") {
	YAML::Node ports = config["Ports"];
	YAML::Node autoSwitch = ports["switch"];
	autoSwitch0 = new DigitalInput(autoSwitch[0].as<int>());
	autoSwitch1 = new DigitalInput(autoSwitch[1].as<int>());
	autoSwitch2 = new DigitalInput(autoSwitch[2].as<int>());
	gyro = new ADXRS453(SPI::kOnboardCS0);

	YAML::Node camera = config["Camera"];
	std::string cameraName = camera["name"].as<std::string>();
	std::shared_ptr<USBCamera> usbCamera(new USBCamera(cameraName, true));
	if (camera["exposure"].as<std::string>() == "auto") {
		usbCamera->SetExposureAuto();
	} else {
		usbCamera->SetExposureManual(camera["exposure"].as<int>());
	}
	usbCamera->SetBrightness(camera["brightness"].as<int>());
	usbCamera->OpenCamera();
	CameraServer::GetInstance()->StartAutomaticCapture(usbCamera);

	ManualTester* manualTester = ManualTester::GetInstance();
	manualTester->Add(GetName(), "auto switch 0", autoSwitch0);
	manualTester->Add(GetName(), "auto switch 1", autoSwitch1);
	manualTester->Add(GetName(), "auto switch 2", autoSwitch2);
	manualTester->Add(GetName(), "gyro", gyro);

	gyro->Start();
}

Otto::~Otto() {
	delete gyro;
}

int Otto::GetAutoModeNumber() {
	bool zero = autoSwitch0->Get();
	bool one = autoSwitch1->Get();
	bool two = autoSwitch2->Get();
	int autoNumber = (zero ? 1 : 0) + (one ? 2 : 0) + (two ? 4 : 0);
	log.Info("Auto Mode is #%d", autoNumber);
	return autoNumber;
}

void Otto::StartAutoCommand() {
	int autoNumber = GetAutoModeNumber();
	log.Info("Running $Auto%d", autoNumber);
	try {
		Kremlin::Get("$Auto" + std::to_string(autoNumber))->Start();
	} catch (std::runtime_error& e) {
		log.Error("That auto doesn't exist, dingus!");
	}
}

double Otto::GetAngle() {
	return gyro->GetAngle();
}

void Otto::ResetAngle() {
	gyro->Reset();
}

void Otto::StartGyroCalibration() {
	gyro->StartCalibration();
}

void Otto::FinishGyroCalibration() {
	gyro->FinishCalibration();
}

} /* namespace tator */

