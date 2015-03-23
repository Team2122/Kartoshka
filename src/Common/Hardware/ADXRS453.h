/**
 * @file ADXRS453.h
 * @author Alex Mikhalev
 * @date Jan 20, 2015
 */

#ifndef ADXRS453_H_
#define ADXRS453_H_

#include <array>
#include <SensorBase.h>
#include <PIDSource.h>
#include <SPI.h>
#include <Task.h>
#include <Notifier.h>
#include "Common/Logger.h"

namespace tator {

/**
 * A sensor class for using an ADXRS453 gyroscope, a SPI gyroscope.
 */
class ADXRS453: public SensorBase, public PIDSource {
private:
	enum MessageType {
		kSensorData = (1L << 29), kRead = (1L << 31), kWrite = (1L << 30)
	};

	enum Bits {
		kP = (1L << 0),
		kChk = (1L << 1),
		kCst = (1L << 2),
		kPwr = (1L << 3),
		kPor = (1L << 4),
		kNvm = (1L << 5),
		kQ = (1L << 6),
		kPll = (1L << 7),
		kFaultBits = kChk | kCst | kPwr | kPor | kNvm | kQ | kPll,
		kDu = (1L << 16),
		kRe = (1L << 17),
		kSpi = (1L << 18),
		kP0 = (1L << 28),
		kWriteBit = (1L << 29),
		kReadBit = (1L << 30)
	};

	enum Status {
		kInvalidData = 0x0, kValidData = 0x1, kTestData = 0x2, kReadWrite = 0x03
	};

	const unsigned kUpdatesPerSecond = 120;
	const double kUpdatePeriod = 1.0 / kUpdatesPerSecond;
	const double kDegreesPerSecondPerLSB = 80.0;
	const static unsigned kCalibrationTicks = 5 * 50; // 5 seconds * 50 per second

public:
	enum Register {
		rRate = 0x00,
		rTem = 0x02,
		rLoCst = 0x04,
		rHiCst = 0x06,
		rQuad = 0x08,
		rFault = 0x0A,
		rPid = 0x0C,
		rSnH = 0x0E,
		rSnL = 0x10
	};

	/**
	 * Creates a new ADXRS453
	 * @param port The SPI port to attach to
	 */
	ADXRS453(SPI::Port port);

	/**
	 * Creates a new ADXRS453
	 * @param spi The spi to use
	 */
	ADXRS453(SPI* spi);

	/**
	 * Destroys this ADXRS453
	 */
	virtual ~ADXRS453();

	/**
	 * Starts the startup thread and update thread.
	 */
	void Start();

	/**
	 * Stops all the threads.
	 */
	void Stop();

	/**
	 * Resets everything
	 */
	void Reset();

	/**
	 * Starts calibrating the gyro
	 */
	void StartCalibration();

	/**
	 * Finishes calibration
	 */
	void FinishCalibration();

	/**
	 * Gets the rate from the gyro
	 * @return The rate in degrees per second, positive is clockwise
	 */
	double GetRate();

	/**
	 * Gets the angle of the gyro
	 * @return The angle of the gyro in degrees
	 */
	double GetAngle();

	/**
	 * Resets the angle of the gyro to zero
	 */
	void ResetAngle();

	uint16_t ReadRegister(Register reg);
	void WriteRegister(Register reg, uint16_t value);
	double GetTemperature();
	uint32_t GetSerialNumber();

	/**
	 * Sets what PID source to use for @c PIDGet
	 * @param pidSource Either @c kRate or @c kAngle
	 */
	void SetPIDSourceParameter(PIDSourceParameter pidSource);

	/**
	 * Gets the value of the gyro for pid
	 * @return Either the angle or rate of the gyro
	 */
	virtual double PIDGet();

protected:
	/**
	 * Fixes odd parity on the value
	 * @param data The input value
	 * @return The output with parity fixed
	 */
	static uint32_t FixParity(uint32_t data);

	/**
	 * Delays a while
	 * @param ms How many milliseconds to delay
	 */
	static void Delay(long int ms);

	/**
	 * Checks the parity and prints if there's an error
	 * @param data The data
	 * @return If it is successful
	 */
	bool CheckParity(uint32_t data);

	/**
	 * Checks the faults of the data and prints if there's an error
	 * @param data The data
	 * @return If it is successful
	 */
	bool CheckFaults(uint32_t data);

	/**
	 * Checks a reponse from the gyro and prints out any errors
	 * @param data The data
	 * @return If it is successful
	 */
	bool CheckResponse(uint32_t data);

	/**
	 * Writes data to the gyro over SPI
	 * @param data the data
	 */
	void Write(uint32_t data);

	/**
	 * Reads data from the gyro over SPI
	 * @return the read data
	 */
	uint32_t Read();

	/**
	 * Reads and writes data to the gyro over SPI
	 * @param data The data to write
	 * @return The read data
	 */
	uint32_t Transfer(uint32_t data);

	/**
	 * Runs the startup routine on the gyro
	 */
	void Startup();

	/**
	 * Reads from the gyro and updates the values from it.
	 */
	void Update();

	static int StartupFunction(int data);
	static void UpdateFunction(void* data);

	Logger log;
	ReentrantSemaphore lock; ///< lock for threaded data
	PIDSourceParameter pidSource; ///< the source for PID data
	double rate; ///< the latest rate read from the gyro, zerod
	double angle; ///< the calculated angle
	uint32_t lastSent; ///< the last data that was sent to the gyro
	std::array<double, kCalibrationTicks> calibValues;
	bool isCalibrating; ///< true if we are calibrating right now
	size_t calibIndex; ///< how many samples are in the zero value
	double zeroRate; ///< the zero value for the rate

	Task* startup; ///< the task that performs the startup routine for the gyro
	Notifier* updater; ///< the notifier that runs the update routine
	Timer* timer; ///< the timer used for measuring the delay between runs
	SPI* spi; ///< the spi class used for communication with the gyro

	DISALLOW_COPY_AND_ASSIGN(ADXRS453);
};

} /* namespace tator */

#endif /* ADXRS453_H_ */
