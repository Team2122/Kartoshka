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
 * An interface for using the ADXRS453 gyroscope, a high-precision SPI gyroscope.
 */
class ADXRS453: public SensorBase, public PIDSource {
private:
	/**
	 * Bit masks that indicate the type of data being written.
	 */
	enum MessageType {
		kSensorData = (1L << 29),	///< Sensor data request
		kRead = (1L << 31),			///< Register read
		kWrite = (1L << 30)			///< Register write
	};

	/**
	 * Bit masks for all of the bits
	 */
	enum Bits {
		/// Odd parity for the entire request/response
		kP = (1L << 0),
		/// Self check bit: faults were generated
		kChk = (1L << 1),
		/// Continuous self-test failure or amplitude detection failed
		kCst = (1L << 2),
		/// Power regulation failed due to overvoltage or undervoltage condition
		kPwr = (1L << 3),
		/// Power-on or reset failed to initialize
		kPor = (1L << 4),
		/// Nonvolatile memory fault
		kNvm = (1L << 5),
		/// Quadrature error
		kQ = (1L << 6),
		/// Phase-locked loop failure
		kPll = (1L << 7),
		/// All of the fault bits together
		kFaultBits = kChk | kCst | kPwr | kPor | kNvm | kQ | kPll,
		/// Data unavailable error. Requests were sent too rapidly
		kDu = (1L << 16),
		/// Request error. Invalid command or register number.
		kRe = (1L << 17),
		/// SPI error. Wrong number of bits transmitted or parity check error
		kSpi = (1L << 18),
		/// Odd parity for the upper bits in a response.
		kP0 = (1L << 28),
		/// Indicates a register write response
		kWriteBit = (1L << 29),
		/// Indicates a register read response
		kReadBit = (1L << 30)
	};

	/**
	 * The status of a response.
	 */
	enum Status {
		kInvalidData = 0x0, ///< The data received is invalid because of a fault
		kValidData = 0x1, 	///< The data received is valid
		kTestData = 0x2,	///< The data received is self test data
		kReadWrite = 0x03 	///< The data received is a read or write response
	};

	/// How many updates per second the update thread performs
	const unsigned kUpdatesPerSecond = 120;
	/// The delay in seconds between updates
	const double kUpdatePeriod = 1.0 / kUpdatesPerSecond;
	/// Conversion factor from the data received to degrees per second
	const double kDegreesPerSecondPerLSB = 80.0;
	/// How many ticks to perform the calibration for.
	const static unsigned kCalibrationTicks = 5 * kUpdatesPerSecond;

public:
	/**
	 * Registers that can be read or written
	 * @see ReadRegister
	 * @see WriteRegister
	 */
	enum Register {
		rRate = 0x00,	///< Temperature compensated rate data
		rTem = 0x02,	///< The temperature of the device
		rLoCst = 0x04,	///< Self test data low
		rHiCst = 0x06,	///< Self test data high
		rQuad = 0x08,	///< Quadrature error
		rFault = 0x0A,	///< Fault bits
		rPid = 0x0C, 	///< Part id
		rSnH = 0x0E, 	///< Serial number high
		rSnL = 0x10  	///< Serial number low
	};

	/**
	 * Creates a new ADXRS453 interface
	 * @param port The SPI port to attach to
	 */
	ADXRS453(SPI::Port port);

	/**
	 * Creates a new ADXRS453 interface
	 * @param spi The SPI object to use
	 */
	ADXRS453(SPI* spi);

	/**
	 * Destroys this ADXRS453
	 */
	virtual ~ADXRS453();

	/**
	 * Starts the startup thread and update thread. Initializes the gyro.
	 */
	void Start();

	/**
	 * Stops all the threads.
	 */
	void Stop();

	/**
	 * Resets everything, including calibration values, current rate and angle.
	 */
	void Reset();

	/**
	 * Starts calibrating the gyro.
	 */
	void StartCalibration();

	/**
	 * Finishes calibration. Averages the last five seconds of values and uses
	 * that for calibration.
	 */
	void FinishCalibration();

	/**
	 * Gets the rate from the gyro
	 * @return The rate in degrees per second, positive is clockwise
	 */
	double GetRate();

	/**
	 * Gets the angle of the gyro
	 * @return The angle of the gyro in degrees, positive is clockwise
	 */
	double GetAngle();

	/**
	 * Resets the angle of the gyro to zero
	 */
	void ResetAngle();

	/**
	 * Reads from a register
	 * @param reg The register to read from
	 * @return The value read from the register
	 */
	uint16_t ReadRegister(Register reg);

	/**
	 * Writes to a register
	 * @param reg The register to write to
	 * @param value The value to write
	 */
	void WriteRegister(Register reg, uint16_t value);

	/**
	 * Gets the temperature of the device
	 * @return The temperature of the device in degrees celcius
	 */
	double GetTemperature();

	/**
	 * Gets the serial number of the gyro
	 * @return The serial number
	 */
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

private:
	/**
	 * Fixes odd parity on the value
	 * @param data The input value
	 * @return The output with parity fixed
	 */
	static uint32_t FixParity(uint32_t data);

	/**
	 * Delays for a while
	 * @param ms How many milliseconds to delay for
	 */
	static void Delay(long int ms);

	/**
	 * Checks the parity and prints if there's an error
	 * @param data The data
	 * @return False if there is an error
	 */
	bool CheckParity(uint32_t data);

	/**
	 * Checks the faults of the data and prints if there's an error
	 * @param data The data
	 * @return False if there is an error
	 */
	bool CheckFaults(uint32_t data);

	/**
	 * Checks a response from the gyro and prints out any errors
	 * @param data The data
	 * @return False if there is an error
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
	 * The startup routine which initializes the gyro.
	 */
	void Startup();

	/**
	 * The update routine which handles reading from the gyro, updating rate and
	 * angle, and calibration.
	 */
	void Update();

	static int StartupFunction(int data);
	static void UpdateFunction(void* data);

	/// The logger used by this class
	Logger log;

	/// Lock for multithreaded data
	ReentrantSemaphore lock;

	/// The source for PID data
	PIDSourceParameter pidSource;

	/// The latest rate read from the gyro, with calibration applied
	double rate;

	/// The calculated angle
	double angle;

	/// The last data that was sent to the gyro
	uint32_t lastSent;

	/// The values used for calibration
	std::array<double, kCalibrationTicks> calibValues;

	/// True if we are calibrating right now
	bool isCalibrating;

	/// How many samples are in the zero value
	size_t calibIndex;

	/// The calibration value for the rate
	double zeroRate;

	/// The task that performs the startup routine for the gyro
	Task* startup;

	/// The notifier that runs the update routine
	Notifier* updater;

	/// The timer used for measuring the delay between updates
	Timer* timer;

	/// The spi class used for communication with the gyro
	SPI* spi;

	DISALLOW_COPY_AND_ASSIGN(ADXRS453);
};

} /* namespace tator */

#endif /* ADXRS453_H_ */
