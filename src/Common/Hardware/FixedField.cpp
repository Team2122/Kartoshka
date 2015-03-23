#include "FixedField.h"

namespace tator {
FixedField::FixedField(uint32_t channel, bool isNPN):
		input(channel) {
	this->NPN = isNPN;
}

bool FixedField::Get() {
	return input.Get() ^ NPN;
}

bool FixedField::IsNPN() {
	return NPN;
}
}
