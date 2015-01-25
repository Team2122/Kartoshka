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
	DigitalInput* lowerSensor;
	DigitalInput* upperSensor;
	DoubleSolenoid* clampPiston;
	Talon* elevator;

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
	Shuttle::Position ShuttlePosition();

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
	 * @param units from -1 (full backwards) to 1 (full forwards)
	 */
	void SetShuttleSpeed(float);

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
};

}

#endif
