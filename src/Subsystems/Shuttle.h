/**
 * @file Shuttle.h
 * @author Daniele Moro
 * @date Jan 24, 2015
 */

#ifndef SHUTTLE_H
#define SHUTTLE_H

#include <WPILib.h>
#include "Common/SubsystemBase.h"
#include "Common/Config.h"

namespace tator {

/**
 * This shuttle take precious totes very high and keep them safe from evil bots
 */
class Shuttle: public SubsystemBase {
private:

	/**
	 * Sensor that senses if totes are here
	 */
	DigitalInput* toteSensor;

	/**
	 * Sensor that senses if Shuttle has reached it's lower limit
	 */
	DigitalInput* lowerLimit;

	/**
	 * Sensor that senses if Shuttle has reached it's upper limit
	 */
	DigitalInput* upperLimit;

	/**
	 * Piston that grabs the totes
	 */
	Solenoid* clampPiston;

	/**
	 * Motor that lifts the shuttle with totes
	 */
	Talon* liftMotor;

	/**
	 * Encoder to read hoe far shuttle has moved
	 */
	Encoder* liftEncoder;

	/**
	 * Power distribution board for detecting if motor has stalled
	 */
	PowerDistributionPanel* pdp;

	/**
	 * Pistons that hold uppermost totes in place
	 */
	Solenoid* fingersPiston;

	/**
	 * This is the speed of the shuttle when going up
	 */
	double upSpeed;

	/**
	 * The speed of the shuttle when going down
	 */
	double downSpeed;

	/**
	 * The speed of the shuttle when holding the totes in place
	 */
	double holdSpeed;

	/**
	 * The value that can be multiplied by how many totes we are holding
	 * to get the speed we need to counteract that force
	 */
	double speedScale;
	uint8_t motorPDPChannel;

	/**
	 * The maximum current that can be sent to the shuttle motors
	 * Is used to know if the motor has stalled
	 */
	double maxMotorCurrent;

	/**
	 * How many totes we are rescuing
	 */
	int totes;

public:
	Shuttle(YAML::Node config);
	virtual ~Shuttle();

	/**
	 * This is the position of the Shuttle itself
	 * kUpper means it is in its uppermost location
	 * kLower means it is in its tote-receiving location
	 */
	enum Position {
		kUpper, kLower, kUnknown
	};

	/**
	 * This is the direction the shuttle has to move
	 */
	enum Speed {
		kUp, kDown, kHold, kStop
	};

	/**
	 * The states of the fingers that hold the uppermost totes in place
	 * kHeld means that they are stationary
	 * kReleased means that they allow the totes to move
	 */
	enum FingersState {
		kHeld = 1, kReleased = 0
	};

	/**
	 * Tells comrade if tote has entered the bot
	 * @return if tote is present
	 */
	bool IsTotePresent();

	/**
	 * Tells comrade where big shuttle is
	 * Returns enum of kUpper, kLower, or kUnknown
	 * @return
	 */
	Shuttle::Position GetLimit();

	/**
	 * Open gates of grand shuttle
	 */
	void OpenProngs();

	/**
	 * Close gates of grand shuttle
	 */
	void CloseProngs();

	/**
	 * Tells grand shuttle how fast to run from Napoleon
	 * @param speed The speed to use.
	 */
	void SetShuttleSpeed(Speed speed);

	/**
	 * Set the speed of the shuttle manually, without the enums
	 * @param speed The rpm the shuttle moves //TODO what units the speed takes in?
	 */
	void SetShuttleSpeed(double speed);

	/**
	 * Gets if stupid usa has stalled motor
	 * @return Yes if evil
	 */
	bool IsStalled();

	/**
	 * Tells comrade how many totes in safety
	 * @return the total number of totes
	 */
	int GetToteCount();

	/**
	 * Adds an int to totes
	 */
	void IncrementToteCount(int);

	/**
	 * Set tote to 0
	 */
	void ResetToteCount();

	/**
	 * Get how many ticks are on the encoder
	 * @return The ticks
	 */
	int32_t GetEncoderTicks();

	/**
	 * Tells sputnik shuttle that home is here
	 */
	void ResetEncoder();

	/**
	 * Releases or holds soviet totes of promised rations to starving Russians
	 * @param state is the enum kHeld (holds totes) or kReleased (releases totes)
	 */
	void SetFingersPiston(FingersState state);

};

}

#endif
