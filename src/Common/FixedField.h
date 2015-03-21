#ifndef FIXEDFIELD_H_
#define FIXEDFIELD_H_

#include <WPILib.h>

namespace tator {
class FixedField {
public:
	FixedField(uint32_t channel, bool isNPN);
	bool Get();
	bool IsNPN();

protected:
	bool NPN;
	DigitalInput input;
};
}

#endif /* FIXEDFIELD_H_ */

