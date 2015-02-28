/**
 * @file HomeClaw.h
 * @date Feb 15, 2015
 * @author Lee Bousfield
 */

#include "ClawLift.h"

#ifndef HOMECLAW_H_
#define HOMECLAW_H_

namespace tator {
class HomeClaw : public ClawLift {
public:
	HomeClaw(std::string name, YAML::Node node):
			ClawLift(name, node) {
		Requires(claw);
	}

	static std::string GetBaseName() {
		return "HomeClaw";
	}

	bool IsFinished() {
		return claw->IsHome();
	}

	void End() {
		ClawLift::End();
		claw->ZeroLiftEncoder();
	}
};
}

#endif /* HOMECLAW_H_ */

