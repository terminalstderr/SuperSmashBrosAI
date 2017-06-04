#pragma once
#include <vector>
#include "State.h"
#include "Action.h"
#include "Experience.h"

#define POSSIBLE_ACTION_COUNT 3

struct NetworkLayer {
	std::vector<std::vector<float>*> weights;
	std::vector<float> biases;


	// TODO -- destructor should free all memory allocated in the init call
	void init(unsigned layer_size, unsigned input_size);
	std::vector<float> *getPerceptronWeights(unsigned i) const;
	const float *getPerceptronBias(unsigned i) const;
	unsigned size() const;
};


class AiEngine
{
private:
	// The input layer will actually just be a preprocessing stage. The first real network layer will be the first hidden layer.
	NetworkLayer output_layer;
	std::vector<float> output_layer_outputs;
	std::vector<NetworkLayer> hidden_layers;
	std::vector<std::vector<float>*> hidden_layer_outputs;


public:
	AiEngine();
	~AiEngine();
	void init(unsigned hidden_layer_count, unsigned hidden_layer_width);
	ActionSharedPtr predict(const StateSharedPtr state);
	void adjustWeights(const ExperienceSharedPtr exp);
};

