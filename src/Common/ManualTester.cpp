#include "ManualTester.h"
#include "ADXRS453.h"

namespace tator {

class ManualTest {
public:
	ManualTest(std::string name, Logger* log) {
		this->name = name;
		this->log = log;
	}
	virtual ~ManualTest() {
	}
	virtual void LogInit() = 0;
	virtual void Execute(Joystick* joystick) = 0;
	virtual void End() = 0;
	// No Interrupted(), Initialize(), or IsFinished(), as these tests should not be statefull
	std::string name;
	Logger* log;
};

class ManualSpeedControllerTest: public ManualTest {
public:
	ManualSpeedControllerTest(std::string name, Logger* log,
			SpeedController* controller, double cap) :
			ManualTest(name, log) {
		this->controller = controller;
		this->cap = cap;
	}
	virtual ~ManualSpeedControllerTest() {
	}
	void LogInit() {
		log->Info("Now testing speed controller %s", name.c_str());
	}
	void Execute(Joystick* joystick) {
		double raw = joystick->GetRawAxis(1);
		double speed = std::min(pow(fabs(raw), 3), cap);
		if (raw < 0) {
			speed *= -1;
		}
		controller->Set(speed);
	}
	void End() {
		controller->Set(0);
	}
	SpeedController* controller;
	double cap;
};

class ManualPIDControllerTest: public ManualTest {
public:
	ManualPIDControllerTest(std::string name, Logger* log,
			PIDController* controller, double maxSetpoint) :
			ManualTest(name, log) {
		this->controller = controller;
		this->maxSetpoint = maxSetpoint;
	}
	virtual ~ManualPIDControllerTest() {
	}
	virtual void LogInit() {
		log->Info("Now testing PID controller %s", name.c_str());
	}
	virtual void Execute(Joystick* joystick) {
		double raw = joystick->GetRawAxis(1);
		double speed = pow(fabs(raw), 3) * maxSetpoint;
		if (raw < 0) {
			speed *= -1;
		}
		controller->SetSetpoint(speed);
	}
	virtual void End() {
		controller->SetSetpoint(0);
	}
	PIDController* controller;
	double maxSetpoint;
};

class ManualSolenoidTest: public ManualTest {
public:
	ManualSolenoidTest(std::string name, Logger* log, Solenoid* solenoid) :
			ManualTest(name, log) {
		this->solenoid = solenoid;
		this->lastPressed = -1;
	}
	virtual ~ManualSolenoidTest() {
	}
	virtual void LogInit() {
		log->Info("Now testing solenoid %s", name.c_str());
		log->Info("Press A (2) to retract, B (3) to extend");
	}
	virtual void Execute(Joystick* joystick) {
		if (joystick->GetRawButton(2)) {
			if (lastPressed != 2) {
				lastPressed = 2;
				log->Info("Setting solenoid %s to false", name.c_str());
				solenoid->Set(false);
			}
		} else if (joystick->GetRawButton(3)) {
			if (lastPressed != 3) {
				lastPressed = 3;
				log->Info("Setting solenoid %s to true", name.c_str());
				solenoid->Set(true);
			}
		} else {
			lastPressed = -1;
		}
	}
	virtual void End() {
	}
	Solenoid* solenoid;
	int lastPressed;
};

class ManualSensorTest: public ManualTest {
public:
	ManualSensorTest(std::string name, Logger* log) :
			ManualTest(name, log) {
		this->wasPressed = false;
	}
	virtual ~ManualSensorTest() {
	}
	virtual void LogValue() = 0;
	virtual void Execute(Joystick* joystick) {
		if (joystick->GetRawButton(2) && !wasPressed) {
			wasPressed = true;
			LogValue();
		} else if (!joystick->GetRawButton(2)) {
			wasPressed = false;
		}
	}
	virtual void End() {
	}
	bool wasPressed;
};

class ManualEncoderTest: public ManualSensorTest {
public:
	ManualEncoderTest(std::string name, Logger* log, Encoder* encoder) :
			ManualSensorTest(name, log) {
		this->encoder = encoder;
		this->wasBPressed = false;
	}
	virtual ~ManualEncoderTest() {
	}
	virtual void LogInit() {
		log->Info("Now testing encoder %s", name.c_str());
		log->Info("Press A (2) to log encoder values");
		log->Info("Press B (3) to zero the encoder");
	}
	virtual void Execute(Joystick* joystick) {
		ManualSensorTest::Execute(joystick);
		if (joystick->GetRawButton(3) && !wasBPressed) {
			wasBPressed = true;
			log->Info("Resetting encoder");
			encoder->Reset();
		} else if (!joystick->GetRawButton(3)) {
			wasBPressed = false;
		}

	}
	virtual void LogValue() {
		log->Info("Encoder is at %.2f and going at %.2f",
				encoder->GetDistance(), encoder->GetRate());
	}
	Encoder* encoder;
	bool wasBPressed;
};

class ManualDigitalInputTest: public ManualSensorTest {
public:
	ManualDigitalInputTest(std::string name, Logger* log, DigitalInput* input) :
			ManualSensorTest(name, log) {
		this->input = input;
	}
	virtual ~ManualDigitalInputTest() {
	}
	virtual void LogInit() {
		log->Info("Now testing digital input %s", name.c_str());
		log->Info("Press A (2) to log the value");
	}
	virtual void LogValue() {
		int state = input->Get();
		const char* strState = state ? "true" : "false";
		log->Info("Input is %s (%d)", strState, state);
	}
	DigitalInput* input;
};

class ManualAnalogPotentiometerTest: public ManualSensorTest {
public:
	ManualAnalogPotentiometerTest(std::string name, Logger* log,
			AnalogPotentiometer* pot) :
			ManualSensorTest(name, log) {
		this->pot = pot;
	}
	virtual ~ManualAnalogPotentiometerTest() {
	}
	virtual void LogInit() {
		log->Info("Now testing analog potentiometer %s", name.c_str());
		log->Info("Press A (2) to log the value");
	}
	virtual void LogValue() {
		log->Info("Potentiometer is at %f", pot->Get());
	}
	AnalogPotentiometer* pot;
};

class ManualADXRS453Test: public ManualSensorTest {
public:
	ManualADXRS453Test(std::string name, Logger* log, ADXRS453* gyro) :
			ManualSensorTest(name, log) {
		this->gyro = gyro;
	}

	void LogInit() override {
		log->Info("Testing gyro %s", name.c_str());
		log->Info("Press A (2) to log the value");
	}

	void LogValue() override {
		log->Info("Gyro angle is %f degrees", gyro->GetAngle());
	}

	ADXRS453* gyro;
};

ManualTester* ManualTester::instance = nullptr;

ManualTester::ManualTester() :
		log("ManualTester") {
	joystick = new Joystick(0);
	lastPressed = -1;
}

ManualTester::~ManualTester() {
}

ManualTester* ManualTester::GetInstance() {
	if (instance == nullptr) {
		instance = new ManualTester();
	}
	return instance;
}

void ManualTester::Execute() {
	if (!doIteratorsExist) {
		doIteratorsExist = true;
		subsystemIt = tests.begin();
		testIt = subsystemIt->second.begin();
		log.Info("Switch subsystems with the left and right triggers");
		log.Info("Switch components with the left and right bumpers");
		log.Info("Starting off with testing subsytem %s",
				subsystemIt->first.c_str());
		(*testIt)->LogInit();
	}
	if (joystick->GetRawButton(7)) { // Left trigger
		if (lastPressed != 6) {
			if (subsystemIt == tests.begin()) {
				log.Warn("Already at the first subsystem");
			} else {
				(*testIt)->End();
				subsystemIt--;
				testIt = subsystemIt->second.begin();
				log.Info("Switching to subsystem %s",
						subsystemIt->first.c_str());
				(*testIt)->LogInit();
			}
		}
		lastPressed = 6;
	} else if (joystick->GetRawButton(8)) { // Right trigger
		if (lastPressed != 7) {
			subsystemIt++;
			if (subsystemIt == tests.end()) {
				subsystemIt--;
				log.Warn("Already at the last subsystem");
			} else {
				(*testIt)->End();
				testIt = subsystemIt->second.begin();
				log.Info("Switching to subsystem %s",
						subsystemIt->first.c_str());
				(*testIt)->LogInit();
			}
		}
		lastPressed = 7;
	} else if (joystick->GetRawButton(5)) { // Left bumper
		if (lastPressed != 4) {
			if (testIt == subsystemIt->second.begin()) {
				log.Warn("Already at the first test");
			} else {
				(*testIt)->End();
				testIt--;
				(*testIt)->LogInit();
			}
		}
		lastPressed = 4;
	} else if (joystick->GetRawButton(6)) { // Right bumper
		if (lastPressed != 5) {
			testIt++;
			if (testIt == subsystemIt->second.end()) {
				testIt--;
				log.Warn("Already at the last test");
			} else {
				(*testIt)->End();
				(*testIt)->LogInit();
			}
		}
		lastPressed = 5;
	} else {
		lastPressed = -1;
	}
	(*testIt)->Execute(joystick);
}

void ManualTester::StopTesting() {
	(*testIt)->End();
}

void ManualTester::Add(std::string subsystem, std::string name,
		SpeedController* controller, double maxSpeed) {
	tests[subsystem].push_back(
			new ManualSpeedControllerTest(name, &log, controller, maxSpeed));
}

void ManualTester::Add(std::string subsystem, std::string name,
		PIDController* controller, double maxSetpoint) {
	tests[subsystem].push_back(
			new ManualPIDControllerTest(name, &log, controller, maxSetpoint));
}

void ManualTester::Add(std::string subsystem, std::string name,
		Solenoid* solenoid) {
	tests[subsystem].push_back(new ManualSolenoidTest(name, &log, solenoid));
}

void ManualTester::Add(std::string subsystem, std::string name,
		Encoder* encoder) {
	tests[subsystem].push_back(new ManualEncoderTest(name, &log, encoder));
}

void ManualTester::Add(std::string subsystem, std::string name,
		DigitalInput* input) {
	tests[subsystem].push_back(new ManualDigitalInputTest(name, &log, input));
}

void ManualTester::Add(std::string subsystem, std::string name,
		AnalogPotentiometer* pot) {
	tests[subsystem].push_back(
			new ManualAnalogPotentiometerTest(name, &log, pot));
}

void ManualTester::Add(std::string subsystem, std::string name,
		ADXRS453* gyro) {
	tests[subsystem].push_back(new ManualADXRS453Test(name, &log, gyro));
}

}

