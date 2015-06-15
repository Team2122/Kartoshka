/**
 * @file ClawClampToggle.cpp
 */

#include "ClawClampToggle.h"

namespace tator {

COMMAND_CONSTRUCTOR_DEF(ClawClampToggle){}

void ClawClampToggle::Execute() {
	if (claw->GetClampStatus() == Claw::ClampStatus::kDeathGrip) {
		log.Info("Releasing claw clamp");
		claw->SetClampStatus(Claw::ClampStatus::kReleased);
	} else {
		log.Info("Death gripping the claw");
		claw->SetClampStatus(Claw::ClampStatus::kDeathGrip);
	}
}

bool ClawClampToggle::IsFinished() {
	return true;
}

}
