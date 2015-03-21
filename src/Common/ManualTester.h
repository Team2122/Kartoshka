#ifndef MANUALTESTER_H_
#define MANUALTESTER_H_

#include <WPILib.h>
#include "FixedField.h"
#include "Common/Logger.h"

namespace tator {

class ManualTest;
class ADXRS453;

class ManualTester {
public:
	virtual ~ManualTester();
	static ManualTester* GetInstance();

	void Execute();
	void StopTesting();

	virtual void Add(std::string subsystem, std::string name,
			SpeedController* controller, double maxSpeed = 1);
	virtual void Add(std::string subsystem, std::string name,
			PIDController* controller, double maxSetpoint);
	virtual void Add(std::string subsystem, std::string name,
			Solenoid* solenoid);
	virtual void Add(std::string subsystem, std::string name, Encoder* encoder);
	virtual void Add(std::string subsystem, std::string name,
			DigitalInput* input);
	virtual void Add(std::string subsystem, std::string name,
			FixedField* input);
	virtual void Add(std::string subsystem, std::string name,
			AnalogPotentiometer* pot);
	void Add(std::string subsystem, std::string name, ADXRS453* gyro);

protected:
	ManualTester();
	static ManualTester* instance;
	std::map<std::string, std::vector<ManualTest*>> tests;
	std::map<std::string, std::vector<ManualTest*>>::iterator subsystemIt;
	bool doIteratorsExist;
	std::vector<ManualTest*>::iterator testIt;
	Logger log;
	Joystick* joystick;
	int lastPressed;
};

}

#endif /* MANUALTESTER_H_ */

