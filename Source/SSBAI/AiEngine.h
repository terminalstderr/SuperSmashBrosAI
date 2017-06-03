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
	void init(unsigned hidden_layer_count, unsigned hidden_layer_width);
	ActionSharedPtr predict(const StateSharedPtr state);
	void adjustWeights(const ExperienceSharedPtr exp);
};

