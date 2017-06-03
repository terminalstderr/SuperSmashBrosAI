#pragma once
#include <vector>
#include "State.h"
#include "Action.h"
#include "Experience.h"

#define POSSIBLE_ACTION_COUNT 3

struct NetworkLayer {
	std::vector<std::vector<float>*> weights;
	std::vector<float> biases;

	void init(unsigned layer_size, unsigned input_size);
	std::vector<float> *getPerceptronWeights(unsigned i);
	float *getPerceptronBias(unsigned i);
};


class AiEngine
{
private:
	// The input layer will actually just be a preprocessing stage. The first real network layer will be the first hidden layer.
	NetworkLayer output_layer;
	std::vector<NetworkLayer> hidden_layers;

public:
	AiEngine();
	~AiEngine();
	void init(unsigned hidden_layer_count, unsigned hidden_layer_width);
	ActionSharedPtr predict(const StateSharedPtr state);
	void adjustWeights(const ExperienceSharedPtr exp);
};

