/**
 * @file Shuttle.h
 * @author Daniele Moro
 * @date Jan 24, 2015
 */

#ifndef SHUTTLE_H
#define SHUTTLE_H

#include <Talon.h>
#include <Solenoid.h>
#include <Encoder.h>
#include <PowerDistributionPanel.h>
#include "Common/SubsystemBase.h"
#include "Common/Config/Config.h"
#include "Common/Hardware/FixedField.h"

namespace tator {

/**
 * This shuttle take precious totes very high and keep them safe from evil bots
 */
class Shuttle: public SubsystemBase {
	friend class Tester;
private:

	FixedField* toteSensor; ///< Sensor that senses if totes are here (at the lift base)
	FixedField* lowerLimit; ///< Sensor that senses if Shuttle has reached it's lower limit
	FixedField* upperLimit; ///< Sensor that senses if Shuttle has reached it's upper limit
	Solenoid* clampPiston; ///< Piston that grabs the totes
	Talon* liftMotor; ///< Motor that lifts the shuttle with totes
	Encoder* liftEncoder; ///< Encoder to read how far shuttle has moved
	Solenoid* fingersPiston; ///< Pistons that hold uppermost totes in place
	std::vector<double> upSpeeds; ///< The speeds of the shuttle when it is going up with 0-4 totes on the ratchets
	double downSpeed; ///< The speed of the shuttle when going down
	double holdSpeed; ///< The speed of the shuttle when holding the totes in place

	unsigned toteHeight; ///< How high of a stack we want to build
	unsigned totesHeld; ///< How many totes the robot is holding
	unsigned totesRatcheted; ///< How many totes are being held by the fingers

public:
	const size_t kNumUpSpeeds = 5;

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
	bool HasToteAtShuttleBase();

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

	double GetShuttleSpeed(Speed speed);

	/**
	 * Gets the height of the stack we want to build
	 * @return Number of totes
	 */
	int GetDesiredTotes();

	/**
	 * Sets the height of the stack we want to build
	 * @param New height
	 */
	void SetDesiredTotes(int height);

	/**
	 * Tells comrade how many totes in safety
	 * @return the total number of totes in the robot
	 */
	int GetTotesHeld();

	/**
	 * Update number of totes in safety with information from comrade
	 * @param The new count of totes
	 */
	void SetTotesHeld(int);

	/**
	 * Tells comrade how many totes are ratcheted
	 * @return the number of totes ratcheted
	 */
	int GetTotesRatcheted();

	/**
	 * Zeros the number of totes ratcheted
	 */
	void ZeroTotesRatcheted();

	/**
	 * Preform calculations to determine the number of totes ratcheted
	 */
	void UpdateTotesRatcheted();

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
