/**
 * @file HomeClaw.h
 * @date Feb 15, 2015
 * @author Lee Bousfield
 */

#ifndef HOMECLAW_H_
#define HOMECLAW_H_

#include "CommandBase.h"
#include "ClawLift.h"

namespace tator {

class HomeClaw: public ClawLift {
public:
	HomeClaw(std::string name, YAML::Node node) :
			ClawLift(name, node) {
		Requires(claw);
	}

	static std::string GetBaseName() {
		return "HomeClaw";
	}

protected:
	bool IsFinished() override {
		return claw->IsHome();
	}

	void End() override {
		ClawLift::End();
		claw->ZeroLiftEncoder();
	}
};

}

#endif /* HOMECLAW_H_ */

