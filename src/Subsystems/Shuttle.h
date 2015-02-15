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
	DigitalInput* toteSensor;
	DigitalInput* lowerLimit;
	DigitalInput* upperLimit;
	Solenoid* clampPiston;
	Talon* liftMotor;
	Encoder* liftEncoder;
	PowerDistributionPanel* pdp;
	Solenoid* fingersPiston;

	double upSpeed;
	double downSpeed;
	double holdSpeed;
	double speedScale;
	uint8_t motorPDPChannel;
	double maxMotorCurrent;

	/**
	 * How many totes we are rescuing
	 */
	int totes;

public:
	Shuttle(YAML::Node config);
	virtual ~Shuttle();

	enum Position {
		kUpper, kLower, kUnknown
	};

	enum Speed {
		kUp, kDown, kHold, kStop
	};

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
