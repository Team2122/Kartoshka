/**
 * @file Robot.h
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include "Common/Logger.h"
#include "Common/Tester/Tester.h"
#include "CommandBase.h"
#include <IterativeRobot.h>

namespace tator {

class Claw;
class Drive;
class Shuttle;
class Thief;
class ToteFeed;
class Otto;

typedef CommandBase<class Robot> RobotCommand;

/**
 * The main robot class.
 */
class Robot: public IterativeRobot {
public:
	Robot();
	virtual ~Robot();

	virtual void RobotInit();

	virtual void DisabledInit();
	virtual void AutonomousInit();
	virtual void TeleopInit();
	virtual void TestInit();

	virtual void DisabledPeriodic();
	virtual void AutonomousPeriodic();
	virtual void TeleopPeriodic();
	virtual void TestPeriodic();

	Scheduler *scheduler;
	Tester *tester;

	Claw* claw;
	Drive* drive;
	Shuttle* shuttle;
	Thief* thief;
	ToteFeed* toteFeed;
	Otto* otto;

protected:
	Logger log;

	void InitSubsystems(YAML::Node subsystems);

private:
	bool hasRunAuto;
};

} /* namespace tator */

#endif /* ROBOT_H_ */
