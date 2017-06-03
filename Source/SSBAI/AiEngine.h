#pragma once
#include <vector>
#include "State.h"
#include "Action.h"
#include "Experience.h"

class AiEngine
{
private:
	std::vector<float> weights;
public:
	AiEngine();
	~AiEngine();
	ActionSharedPtr predict(const StateSharedPtr state);
	void adjustWeights(const ExperienceSharedPtr exp);
};

