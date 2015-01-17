/**
 * @file Robot.h
 * @author Alex Mikhalev
 * @date Jan 17, 2015
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <WPILib.h>

namespace tator {

/**
 * The main robot class.
 */
class Robot : public IterativeRobot {
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

private:
	Scheduler *scheduler;
};

} /* namespace tator */

#endif /* ROBOT_H_ */
