#pragma once
#include <vector>
#include <fstream>

#include "State.h"
#include "Action.h"
#include "Experience.h"

#define POSSIBLE_STATE_COUNT 24
#define POSSIBLE_ACTION_COUNT 7
#define OUT_LEFT_RIGHT 0
#define OUT_UP_DOWN 1
#define OUT_SPEED 2
#define OUT_SHIELD 3
#define OUT_ATTACK 4
#define OUT_SPECIAL 5
#define OUT_JUMP 6

struct NetworkLayer {
	std::vector<std::vector<float>*> weights;
	std::vector<float> biases;


	// TODO -- destructor should free all memory allocated in the init call
	void init(unsigned layer_size, unsigned input_size);
	void rand();
	std::vector<float> *getPerceptronWeights(unsigned i) const;
	const float *getPerceptronBias(unsigned i) const;
	unsigned size() const;
	unsigned input_size() const;
};


class AiEngine
{
private:
	// The input layer will actually just be a preprocessing stage. The first real network layer will be the first hidden layer.
	NetworkLayer output_layer;
	std::vector<float> output_layer_outputs;
	std::vector<NetworkLayer> hidden_layers;
	std::vector<std::vector<float>*> hidden_layer_outputs;
	unsigned hidden_layer_count;
	unsigned hidden_layer_width;

	std::shared_ptr<std::vector<float>> get_input_layer(StateSharedPtr state);
	void compute_output_layer(const NetworkLayer &layer, const std::vector<float> *input_layer, std::vector<float> *output_layer);
	ActionSharedPtr get_action(std::vector<float> network_outputs);


public:
	AiEngine();
	~AiEngine();
	void init(unsigned hidden_layer_count, unsigned hidden_layer_width);
	void rand();
	ActionSharedPtr predict(const StateSharedPtr state);
	void adjustWeights(const ExperienceSharedPtr exp);
};

