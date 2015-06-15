#ifndef MANUALTESTER_H_
#define MANUALTESTER_H_

#include "Common/Hardware/FixedField.h"
#include "Common/Logger.h"
#include <map>
#include <vector>
#include <memory>
#include <bitset>

class SpeedController;
class PIDController;
class Solenoid;
class Encoder;
class DigitalInput;
class AnalogPotentiometer;
class Joystick;
class DriverStation;

namespace tator {

class FixedField;
class ADXRS453;

class ManualTest {
public:
	ManualTest(const std::string& name, Logger& log);
	virtual ~ManualTest();

	std::string GetName();

	virtual void Initialize() = 0;
	virtual void Execute(uint32_t pressedButton, double axisValue) = 0;
	virtual void End() = 0;

protected:
	std::string name;
	Logger& log;
};

class ManualTester {
private:
	class TesterSubsystem {
	public:
		TesterSubsystem(const std::string& name, ManualTester* manualTester);
		TesterSubsystem& Add(const std::string& name,
				SpeedController* controller, double maxSpeed = 1);
		TesterSubsystem& Add(const std::string& name, PIDController* controller,
				double maxSetpoint);
		TesterSubsystem& Add(const std::string& name, Solenoid* solenoid);
		TesterSubsystem& Add(const std::string& name, Encoder* encoder);
		TesterSubsystem& Add(const std::string& name, DigitalInput* input);
		TesterSubsystem& Add(const std::string& name, FixedField* input);
		TesterSubsystem& Add(const std::string& name, AnalogPotentiometer* pot);
		TesterSubsystem& Add(const std::string& name, ADXRS453* gyro);

	private:
		std::string subsystem;
		ManualTester* manualTester;
	};

public:
	static const uint32_t kStickPort = 1;
	static const uint32_t kMaxButtons = 32;

	virtual ~ManualTester();
	static ManualTester* GetInstance();

	void Initialize();
	void Run();
	void Disable();

	TesterSubsystem Subsystem(const std::string& subsystem);

	void Add(const std::string& subsystem, const std::string& name,
			ManualTest* test);

private:
	ManualTester();
	static ManualTester* instance;

	typedef std::unique_ptr<ManualTest> TestPtr;
	typedef std::vector<TestPtr> Tests;
	typedef std::map<std::string, Tests> Subsystems;
	typedef std::bitset<kMaxButtons> Buttons;

	Subsystems subsystems;
	Subsystems::iterator currentSubsystem;
	Tests::iterator currentTest;

	Logger log;
	Joystick* joystick;
	DriverStation* ds;
	Buttons lastButtons;

	uint32_t ReadButton();
};

}

#endif /* MANUALTESTER_H_ */
