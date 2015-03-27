#ifndef UNLOADHEIGHTDICTATOR_H
#define UNLOADHEIGHTDICTATOR_H

#include "CommandBase.h"
#include "Subsystems/Claw.h"
#include "Subsystems/Shuttle.h"

namespace tator {

class UnloadHeightDictator: public CommandBase {
public:
	UnloadHeightDictator(std::string name, YAML::Node config) :
			CommandBase(name) {
		Requires(claw);
	}

	static std::string GetBaseName() {
		return "UnloadHeightDictator";
	}

protected:
	void Initialize() override {
		switch (shuttle->GetDesiredTotes()) {
		case 1:
			vladimir = "$PlacementStack2";
			break;
		case 2:
			vladimir = "$PlacementStack2";
			break;
		case 3:
			vladimir = "$PlacementStack3";
			break;
		case 4:
			vladimir = "$PlacementStack4";
			break;
		case 5:
			vladimir = "$PlacementStack5";
			break;
		case 6:
			vladimir = "$PlacementStack6";
			break;
		default:
			vladimir = "$PlacementStack6";
			break;
		}
		Command* command = Kremlin::Get(vladimir);
		command->Start();
		CommandBase::Initialize();
	}

	void Execute() override {
	}

	bool IsFinished() override {
		return true;
	}

	void End() override {
		CommandBase::End();
	}

	void Interrupted() override {
		CommandBase::Interrupted();
	}

private:
	std::String vladimir;
};

}

#endif /* CLAWSMARTROLLERS_H */
