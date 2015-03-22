/**
 * @file TopClaw.h
 * @date Feb 15, 2015
 * @author Lee Bousfield
 */

#ifndef TOPCLAW_H_
#define TOPCLAW_H_

#include "CommandBase.h"
#include "ClawLift.h"

namespace tator {
class TopClaw: public ClawLift {
public:
	TopClaw(std::string name, YAML::Node node) :
			ClawLift(name, node) {
	}

	static std::string GetBaseName() {
		return "TopClaw";
	}

protected:
	bool IsFinished() override {
		return claw->IsAtTop();
	}
};
}

#endif /* TOPCLAW_H_ */
