#include "ManualTester.h"

#include <SpeedController.h>
#include <PIDController.h>
#include <Solenoid.h>
#include <Encoder.h>
#include <DigitalInput.h>
#include <DriverStation.h>
#include <Joystick.h>

#ifndef ANALOGPOTENTIOMETER_H_
#define ANALOGPOTENTIOMETER_H_
#include <AnalogPotentiometer.h>
#endif

#include "Common/Hardware/ADXRS453.h"

namespace tator {

ManualTest::ManualTest(const std::string& name, Logger& log) :
		name(name), log(log) {
}

ManualTest::~ManualTest() {
}

std::string ManualTest::GetName() {
	return name;
}

class ManualSpeedControllerTest: public ManualTest {
public:
	ManualSpeedControllerTest(const std::string& name, Logger& log,
			SpeedController* controller, double cap) :
			ManualTest(name, log) {
		this->controller = controller;
		this->cap = cap;
	}

	void Initialize() override {
		log.Info("Testing speed controller %s", name.c_str());
		log.Info("Press A (2) for 100%% and B (3) for -100%%");
	}

	void Execute(uint32_t pressedButton, double axisValue) override {
		if (pressedButton == 2) {
			controller->Set(1.0);
		} else if (pressedButton == 3) {
			controller->Set(-1.0);
		} else {
			double speed = std::min(pow(fabs(axisValue), 3), cap);
			if (axisValue < 0) {
				speed *= -1;
			}
			controller->Set(speed);
		}
	}

	void End() override {
		controller->Set(0);
	}

private:
	SpeedController* controller;
	double cap;
};

class ManualPIDControllerTest: public ManualTest {
public:
	ManualPIDControllerTest(const std::string& name, Logger& log,
			PIDController* controller, double maxSetpoint) :
			ManualTest(name, log) {
		this->controller = controller;
		this->maxSetpoint = maxSetpoint;
	}

	void Initialize() override {
		log.Info("Testing PID controller %s", name.c_str());
	}

	void Execute(uint32_t pressedButton, double axisValue) override {
		double speed = pow(fabs(axisValue), 3) * maxSetpoint;
		if (axisValue < 0) {
			speed *= -1;
		}
		controller->SetSetpoint(speed);
	}

	void End() override {
		controller->SetSetpoint(0);
	}

private:
	PIDController* controller;
	double maxSetpoint;
};

class ManualSolenoidTest: public ManualTest {
public:
	ManualSolenoidTest(const std::string& name, Logger& log, Solenoid* solenoid) :
			ManualTest(name, log), solenoid(solenoid) {
	}

	void Initialize() override {
		log.Info("Testing solenoid %s", name.c_str());
		log.Info("Press A (2) to retract, B (3) to extend");
	}

	void Execute(uint32_t pressedButton, double axisValue) override {
		if (pressedButton == 2) {
			log.Info("Setting solenoid %s to false", name.c_str());
			solenoid->Set(false);
		} else if (pressedButton == 3) {
			log.Info("Setting solenoid %s to true", name.c_str());
			solenoid->Set(true);
		}
	}

	void End() override {
	}

private:
	Solenoid* solenoid;
};

class ManualSensorTest: public ManualTest {
public:
	ManualSensorTest(const std::string& name, Logger& log) :
			ManualTest(name, log) {
	}

	void Initialize() override {
		LogInit();
	}

	void Execute(uint32_t pressedButton, double axisValue) override {
		if (pressedButton == 2) {
			LogValue();
		}
	}

	void End() override {
	}

protected:
	virtual void LogInit() = 0;
	virtual void LogValue() = 0;
};

class ManualEncoderTest: public ManualTest {
public:
	ManualEncoderTest(const std::string& name, Logger& log, Encoder* encoder) :
			ManualTest(name, log), encoder(encoder) {
	}

	void Initialize() override {
		log.Info("Testing encoder %s", name.c_str());
		log.Info("Press A (2) to log the encoder value");
		log.Info("Press B (3) to zero the encoder value");
	}

	void Execute(uint32_t pressedButton, double axisValue) override {
		if (pressedButton == 2) {
			log.Info("Encoder ticks is %d, distance is %.2f, rate is %.2f",
					encoder->GetRaw(), encoder->GetDistance(),
					encoder->GetRate());
		} else if (pressedButton == 3) {
			log.Info("Resetting encoder");
			encoder->Reset();
		}
	}

	void End() override {
	}

private:
	Encoder* encoder;
};

class ManualDigitalInputTest: public ManualSensorTest {
public:
	ManualDigitalInputTest(const std::string& name, Logger& log,
			DigitalInput* input) :
			ManualSensorTest(name, log), input(input) {
	}

	void LogInit() override {
		log.Info("Now testing digital input %s", name.c_str());
		log.Info("Press A (2) to read the value");
	}

	void LogValue() override {
		log.Info("Input reads %s", input->Get() ? "true" : "false");
	}

private:
	DigitalInput* input;
};

class ManualFixedFieldTest: public ManualSensorTest {
public:
	ManualFixedFieldTest(const std::string& name, Logger& log,
			FixedField* input) :
			ManualSensorTest(name, log), input(input) {
	}

	void LogInit() override {
		const char* type = input->IsNPN() ? "NPN" : "PNP";
		log.Info("Testing %s fixed field sensor %s", type, name.c_str());
		log.Info("Press A (2) to read the value");
	}

	void LogValue() override {
		log.Info("Sensor reads %s", input->Get() ? "true" : "false");
	}

private:
	FixedField* input;
};

class ManualAnalogPotentiometerTest: public ManualSensorTest {
public:
	ManualAnalogPotentiometerTest(const std::string& name, Logger& log,
			AnalogPotentiometer* pot) :
			ManualSensorTest(name, log), pot(pot) {
	}

	void LogInit() override {
		log.Info("Testing analog potentiometer %s", name.c_str());
		log.Info("Press A (2) to read the value");
	}

	void LogValue() override {
		log.Info("Potentiometer reads %f", pot->Get());
	}

private:
	AnalogPotentiometer* pot;
};

class ManualADXRS453Test: public ManualSensorTest {
public:
	ManualADXRS453Test(const std::string& name, Logger& log, ADXRS453* gyro) :
			ManualSensorTest(name, log), gyro(gyro) {
	}

	void LogInit() override {
		log.Info("Testing gyro %s", name.c_str());
		log.Info("Press A (2) to read the value");
	}

	void LogValue() override {
		log.Info("Gyro angle reads %f degrees", gyro->GetAngle());
	}

private:
	ADXRS453* gyro;
};

ManualTester::TesterSubsystem::TesterSubsystem(const std::string& name,
		ManualTester* manualTester) :
		subsystem(name), manualTester(manualTester) {
}

ManualTester::TesterSubsystem& ManualTester::TesterSubsystem::Add(
		const std::string& name, SpeedController* controller, double maxSpeed) {
	manualTester->Add(subsystem, name,
			new ManualSpeedControllerTest(name, manualTester->log, controller,
					maxSpeed));
	return *this;
}

ManualTester::TesterSubsystem& ManualTester::TesterSubsystem::Add(
		const std::string& name, PIDController* controller,
		double maxSetpoint) {
	manualTester->Add(subsystem, name,
			new ManualPIDControllerTest(name, manualTester->log, controller,
					maxSetpoint));
	return *this;
}

ManualTester::TesterSubsystem& ManualTester::TesterSubsystem::Add(
		const std::string& name, Solenoid* solenoid) {
	manualTester->Add(subsystem, name,
			new ManualSolenoidTest(name, manualTester->log, solenoid));
	return *this;
}

ManualTester::TesterSubsystem& ManualTester::TesterSubsystem::Add(
		const std::string& name, Encoder* encoder) {
	manualTester->Add(subsystem, name,
			new ManualEncoderTest(name, manualTester->log, encoder));
	return *this;
}

ManualTester::TesterSubsystem& ManualTester::TesterSubsystem::Add(
		const std::string& name, DigitalInput* input) {
	manualTester->Add(subsystem, name,
			new ManualDigitalInputTest(name, manualTester->log, input));
	return *this;
}

ManualTester::TesterSubsystem& ManualTester::TesterSubsystem::Add(
		const std::string& name, FixedField* input) {
	manualTester->Add(subsystem, name,
			new ManualFixedFieldTest(name, manualTester->log, input));
	return *this;
}

ManualTester::TesterSubsystem& ManualTester::TesterSubsystem::Add(
		const std::string& name, AnalogPotentiometer* pot) {
	manualTester->Add(subsystem, name,
			new ManualAnalogPotentiometerTest(name, manualTester->log, pot));
	return *this;
}

ManualTester::TesterSubsystem& ManualTester::TesterSubsystem::Add(
		const std::string& name, ADXRS453* gyro) {
	manualTester->Add(subsystem, name,
			new ManualADXRS453Test(name, manualTester->log, gyro));
	return *this;
}

ManualTester* ManualTester::instance = nullptr;

ManualTester::ManualTester() :
		log("ManualTester") {
	joystick = Joystick::GetStickForPort(kStickPort);
	ds = DriverStation::GetInstance();
}

ManualTester::~ManualTester() {
}

ManualTester* ManualTester::GetInstance() {
	if (instance == nullptr) {
		instance = new ManualTester();
	}
	return instance;
}

void ManualTester::Initialize() {
	currentSubsystem = subsystems.begin();
	currentTest = currentSubsystem->second.begin();
	log.Info("Switch subsystems with the left and right triggers");
	log.Info("Switch components with the left and right bumpers");
	log.Info("Starting off with testing subsytem %s",
			currentSubsystem->first.c_str());
	(*currentTest)->Initialize();
}

void ManualTester::Run() {
	uint32_t button = ReadButton();
	if (button == 7) { // Left trigger
		if (currentSubsystem == subsystems.begin()) {
			log.Warn("Already at the first subsystem");
		} else {
			(*currentTest)->End();
			currentSubsystem--;
			currentTest = currentSubsystem->second.begin();
			log.Info("Switching to subsystem %s",
					currentSubsystem->first.c_str());
			(*currentTest)->Initialize();
		}
	} else if (button == 8) { // Right trigger
		currentSubsystem++;
		if (currentSubsystem == subsystems.end()) {
			currentSubsystem--;
			log.Warn("Already at the last subsystem");
		} else {
			(*currentTest)->End();
			currentTest = currentSubsystem->second.begin();
			log.Info("Switching to subsystem %s",
					currentSubsystem->first.c_str());
			(*currentTest)->Initialize();
		}
	} else if (button == 5) { // Left bumper
		if (currentTest == currentSubsystem->second.begin()) {
			log.Warn("Already at the first test");
		} else {
			(*currentTest)->End();
			currentTest--;
			(*currentTest)->Initialize();
		}
	} else if (button == 6) { // Right bumper
		currentTest++;
		if (currentTest == currentSubsystem->second.end()) {
			currentTest--;
			log.Warn("Already at the last test");
		} else {
			(*currentTest)->End();
			(*currentTest)->Initialize();
		}
	}
	(*currentTest)->Execute(button, joystick->GetRawAxis(1));
}

void ManualTester::Disable() {
	(*currentTest)->End();
}

ManualTester::TesterSubsystem ManualTester::Subsystem(
		const std::string& subsystem) {
	return TesterSubsystem(subsystem, this);
}

void ManualTester::Add(const std::string& subsystem, const std::string& name,
		ManualTest* test) {
	subsystems[subsystem].push_back(TestPtr(test));
}

uint32_t ManualTester::ReadButton() {
	Buttons buttons = ds->GetStickButtons(kStickPort);
	// buttons weren't pressed and are now
	Buttons pressedButtons = buttons & ~lastButtons;
	lastButtons = buttons;
	if (pressedButtons.any()) {
		for (uint32_t i = 0; i < kMaxButtons; ++i) {
			if (pressedButtons[i]) {
				return i;
			}
		}
	}
	return 0;
}

}

