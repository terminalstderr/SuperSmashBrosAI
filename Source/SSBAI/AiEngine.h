#pragma once
#include <vector>
#include "State.h"
#include "Action.h"
#include "Experience.h"

struct NetworkLayer {
	std::vector<std::vector<float>> weights;
	std::vector<float> biases;

	void init(unsigned layer_size, unsigned input_size);
	std::vector<float> *getWeights(unsigned i);
	float *getBias(unsigned i);
};

class AiEngine
{
private:
	std::vector<std::vector<float>> hidden_layer_weights;
public:
	AiEngine();
	~AiEngine();
	void init(unsigned hidden_layer_count, unsigned hidden_layer_width);
	ActionSharedPtr predict(const StateSharedPtr state);
	void adjustWeights(const ExperienceSharedPtr exp);
};

