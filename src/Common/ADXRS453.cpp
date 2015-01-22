/**
 * @file ADXRS453.cpp
 * @author Alex Mikhalev
 * @date Jan 20, 2015
 */

#include "ADXRS453.h"

#include <byteswap.h>
#include <cstring>
#include <stdarg.h>
#include <ctime>
#include <algorithm>

namespace tator {

ADXRS453::ADXRS453(SPI::Port port) :
		ADXRS453(new SPI(port)) {
}

ADXRS453::ADXRS453(SPI* spi) :
		log("ADXRS453"), spi(spi) {
	startup = new Task("ADXRS453_Startup", (FUNCPTR) &StartupFunction);
	updater = new Notifier(&UpdateFunction, this);
	timer = new Timer();
	pidSource = kAngle;
	Reset();
}

ADXRS453::~ADXRS453() {
	delete spi;
	delete startup;
	delete updater;
	delete timer;
}

double ADXRS453::GetRate() {
	Synchronized sync(lock);
	return rate;
}

double ADXRS453::GetAngle() {
	Synchronized sync(lock);
	return angle;
}

void ADXRS453::Reset() {
	Synchronized sync(lock);
	rate = 0;
	angle = 0;
	zeroRate = 0;
	isCalibrating = false;
	calibValues.fill(0);
	calibIndex = 0;
	timer->Reset();
}

void ADXRS453::Write(uint32_t data) {
	lastSent = data;
	data = bswap_32(data); // convert from capitalist endian to russian byte ordering
	uint8_t send[4];
	memcpy(send, &data, sizeof(send)); // copy the message into a buffer
	spi->Write(send, sizeof(send)); // send it
}

uint32_t ADXRS453::Read() {
	uint8_t recv[4];
	spi->Read(false, recv, sizeof(recv)); // read into the buffer
	uint32_t ret;
	memcpy(&ret, recv, sizeof(ret)); // copy it into a value
	ret = bswap_32(ret); // glorious mother russian endian
	return ret;
}

uint32_t ADXRS453::Transfer(uint32_t data) {
	lastSent = data;
	data = bswap_32(data);
	uint8_t send[4], recv[4];
	memcpy(send, &data, sizeof(send));
	spi->Transaction(send, recv, sizeof(send));
	uint32_t ret;
	memcpy(&ret, recv, sizeof(ret));
	ret = bswap_32(ret); // glorious mother russian endian
	return ret;
}

void ADXRS453::Start() {
	log.Info("Starting");
	startup->Start((uint32_t) this); // pass a pointer to this to the static function
}

void ADXRS453::Stop() {
	log.Info("Stopping");
	startup->Stop();
	updater->Stop();
	timer->Stop();
}

void ADXRS453::ResetAngle() {
	Synchronized sync(lock);
	angle = 0;
}

int ADXRS453::StartupFunction(int data) {
	ADXRS453* self = (ADXRS453*) data;
	self->Startup();
	return 0;
}

void ADXRS453::Startup() {
	uint32_t send, recv;
	send = FixParity(kSensorData | kChk);
	Write(send);
	Delay(50); // in the spec
	send = FixParity(kSensorData);
	Write(send);
	Delay(50);
	recv = Transfer(send);
	if ((recv & kFaultBits) != kFaultBits) // assert that all faults are set
		log.Error("Startup self test failed: %#x", recv);
	Delay(50);
	Write(send);
	Delay(50);
	timer->Start();
	updater->StartPeriodic(kUpdatePeriod);
}

void ADXRS453::StartCalibration() {
	calibValues.fill(0.0);
	calibIndex = 0;
	isCalibrating = true;
}

void ADXRS453::FinishCalibration() {
	double sum = 0;
	for (auto it = calibValues.begin(); it != calibValues.end(); ++it) {
		sum += *it;
	}
	zeroRate = sum / kCalibrationTicks;
	log.Info("Finished calibrating gyro. Offset is %f", zeroRate);
	isCalibrating = false;
}

void ADXRS453::UpdateFunction(void* data) {
	ADXRS453* self = (ADXRS453*) data;
	self->Update();
}

void ADXRS453::Update() {
	Synchronized sync(lock);
	uint32_t send, recv;
	send = FixParity(kSensorData);
	if (lastSent != send) // optimization, because of data latching
		Write(send);
	recv = Transfer(send);
	if (!CheckResponse(recv))
		return;
	int16_t rawRate = (recv >> 10) & 0xFFFF; // bits 10-25
	rate = rawRate / kDegreesPerSecondPerLSB;
	double ellapsed = timer->Get();
	timer->Reset();
	if (ellapsed > .5) // if it's more than half a second, it's probably trash
		ellapsed = 0;
	if (isCalibrating) {
		calibValues[calibIndex] = rate;
		++calibIndex %= kCalibrationTicks;
	} else {
		rate -= zeroRate; // zero it
		angle += rate * ellapsed; // integrate it
	}
}

void ADXRS453::Delay(long int ms) {
	struct timespec t1 = { 0, ms * 1000 * 1000 }; // ms to ns
	nanosleep(&t1, nullptr);
}

uint32_t ADXRS453::FixParity(uint32_t data) {
	data &= ~kP;
	return data | (__builtin_parity(data) ? 0 : kP); // gcc only
}

bool ADXRS453::CheckParity(uint32_t data) {
	int p1 = __builtin_parity(data);
	int p0 = __builtin_parity(data >> 16); // only upper 16 bits
	if (!p1 || !p0) { // check odd parity
		log.Error("Parity check failed on response: %#x", data);
		return false;
	}
	return true;
}

bool ADXRS453::CheckFaults(uint32_t data) {
	bool hasFaults = (data & kFaultBits) != 0;
	if (hasFaults) {
		log.Warn("Faults detected:");
	} else {
		return true;
	}
	if (data & kChk) {
		log.Warn(" * Self test enabled");
		return false;
	}
	if (data & kCst)
		log.Error(" * Continuous self test fault");
	if (data & kPwr)
		log.Error(" * Power fault");
	if (data & kPor)
		log.Error(" * Non-volatile programming fault");
	if (data & kNvm)
		log.Error(" * Non-volatile checksum fault");
	if (data & kQ)
		log.Error(" * Quadrature calculation fault");
	if (data & kPll)
		log.Error(" * Phase locked loop fault");
	return false;
}

bool ADXRS453::CheckResponse(uint32_t data) {
	if (!CheckParity(data))
		return false;
	uint8_t status = (data >> 26) & 0b11;
	switch (status) {
	case kInvalidData:
		log.Warn("Invalid data received");
		CheckFaults(data);
		return false;
	case kValidData:
	case kTestData:
		return CheckFaults(data);
	case kReadWrite:
		if (data & (kReadBit | kWriteBit))
			return true;
		log.Error("Read/Write error: ");
		if (data & kSpi)
			log.Error(" * SPI error");
		if (data & kRe)
			log.Error(" * Request error");
		if (data & kDu)
			log.Error(" * Data unavailable");
		CheckFaults(data);
		return false;
	}
	return false;
}

double ADXRS453::PIDGet() {
	switch (pidSource) {
	case kAngle:
		return GetAngle();
	case kRate:
		return GetRate();
	default:
		return 0;
	}
}

void ADXRS453::SetPIDSourceParameter(PIDSourceParameter pidSource) {
	this->pidSource = pidSource;
}

} /* namespace tator */
