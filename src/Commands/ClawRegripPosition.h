/**
 * @file ClawRegripPosition
 * @date Feb 15, 2015
 * @author Lee Bousfield
 */

#include "ClawLift.h"

#ifndef CLAWREGRIPPOSITION_H_
#define CLAWREGRIPPOSITION_H_

namespace tator {
class ClawRegripPosition: public ClawLift {
public:
	ClawRegripPosition(std::string name, YAML::Node node) :
			ClawLift(name, node) {
	}

	static std::string GetBaseName() {
		return "ClawRegripPosition";
	}

	bool IsFinished() {
		return !claw->HasContainer();
	}

	void End() {
		ClawLift::End();
	}
};
}

#endif /* CLAWREGRIPPOSITION_H_ */

