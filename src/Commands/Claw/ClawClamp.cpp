/**
 * @file ClawClamp.cpp
 */

#include "ClawClamp.h"

namespace tator {

COMMAND_CONSTRUCTOR_DEF(ClawClamp) {
	clampStatus = (Claw::ClampStatus) config["status"].as<int>();
}

void ClawClamp::Execute() {
	claw->SetClampStatus(clampStatus);
}

bool ClawClamp::IsFinished() {
	return true;
}

}
