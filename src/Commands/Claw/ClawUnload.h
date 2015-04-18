/**
 * @file ClawUnload.h
 * @author Alex Mikhalev
 * @date Apr 18, 2015
 */

#ifndef CLAWUNLOAD_H_
#define CLAWUNLOAD_H_

namespace tator {

class ClawUnload: public CommandBase {
public:
	ClawUnload(std::string name, YAML::Node config) :
			CommandBase(name) {
		presses = 0;
		clawToHeightTop = Kremlin::Get("ClawToHeightTop");
		clawToAngleFront = Kremlin::Get("ClawToAngleFront");
	}

	static std::string GetBaseName() {
		return "ClawUnload";
	}

protected:
	void Initialize() override {
		CommandBase::Initialize();
		presses++;
	}

	void Execute() override {
		// If this is the first time we have pressed the button
		if (presses == 1) {
			// Run the claw to the top
			clawToHeightTop->Start();
		}
		// If this is the second time we have pressed the button
		else if (presses == 2) {
			// And the claw has gotton to the top already
			if (!clawToHeightTop->IsRunning()) {
				clawToAngleFront->Start();
			}
		}
	}

	bool IsFinished() override {
		// If this is the second time we have pressed, but the claw isn't to the
		// top yet, then we are _not_ finished
		return !(presses == 2 && clawToHeightTop->IsRunning());
	}

	void End() override {
		CommandBase::End();
		// If we have pressed it at least 2 times
		if (presses >= 2) {
			// Reset the number of presses to 0
			presses = 0;
		}
	}

private:
	unsigned presses;
	Command* clawToHeightTop;
	Command* clawToAngleFront;
};

}

#endif /* CLAWUNLOAD_H_ */
