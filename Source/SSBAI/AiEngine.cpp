#include "stdafx.h"
#include "AiEngine.h"
#include "Utility.h"
#include <random>


AiEngine::AiEngine()
{
}


AiEngine::~AiEngine()
{
}

void AiEngine::init(unsigned hidden_layer_count, unsigned hidden_layer_width)
{
	// Handle output layer perceptrons (Network Layer) and output vector
	output_layer.init(POSSIBLE_ACTION_COUNT, hidden_layer_width);
	output_layer_outputs.resize(POSSIBLE_ACTION_COUNT, 0.0);
	// Handle hidden layers' perceptrons (Network Layers) and output vectors
	for (unsigned i = 0; i < hidden_layer_count; i++) {
		NetworkLayer nl;
		// The first layer of the network only has POSSIBLE_STATE_COUNT inputs, not the full hidden_layer_width
		if (i == 0) 
		{
			nl.init(hidden_layer_width, POSSIBLE_STATE_COUNT);
		}
		else 
		{
			nl.init(hidden_layer_width, hidden_layer_width);
		}
		hidden_layers.push_back(nl);
		std::vector<float> *hlo = new std::vector<float>(hidden_layer_width, 0.0);
		hidden_layer_outputs.push_back(hlo);
	}
	// Setup the meta variables for this data structure
	this->hidden_layer_count = hidden_layer_count;
	this->hidden_layer_width = hidden_layer_width;
}

void AiEngine::rand()
{
	// Handle output layer perceptrons (Network Layer) and output vector
	output_layer.rand();
	// Handle hidden layers' perceptrons (Network Layers) and output vectors
	for (unsigned i = 0; i < hidden_layer_count; i++) {
		hidden_layers[i].rand();
	}
}

// TODO: The outer for loop can be parallelized.
void AiEngine::compute_output_layer(const NetworkLayer &layer, const std::vector<float> *input_layer, std::vector<float> *output_layer)
{
	// for every perceptron in the layer
	for (unsigned p = 0; p < layer.size(); ++p)
	{
		// compute the magnitude of this perceptron
		std::vector<float>	*weights = layer.getPerceptronWeights(p);
		const float			*bias = layer.getPerceptronBias(p);
		float accumulator = 0.0;
		for (unsigned i = 0; i < weights->size(); ++i)
		{
			accumulator += (*input_layer)[i] * (*weights)[i];
		}
		// threshold check
		(*output_layer)[p] = accumulator > (*bias) ? 1.0 : 0.0;
	}
}

ActionSharedPtr AiEngine::get_action(std::vector<float> network_outputs)
{
	ActionSharedPtr action = ActionSharedPtr(new Action());
	// First 3 outputs dictate how the AI moves
	float lr = network_outputs[OUT_LEFT_RIGHT];
	float ud = network_outputs[OUT_UP_DOWN];
	float s = network_outputs[OUT_SPEED];
	float x = (lr == 0.0 ? -1.0f : 1.0f) * (s == 0.0 ? 0.5 : 1.0f);
	float y = (lr == 0.0 ? -1.0f : 1.0f) * (s == 0.0 ? 0.5 : 1.0f);
	Vector2 movement;
	movement.update(x, y);
	action->Move(movement);

	// Next 4 outputs dictates whether AI does Sheild, Attack, Special, Jump
	if (network_outputs[OUT_ATTACK] == 1.0)
		action->Attack();
	if (network_outputs[OUT_SHIELD] == 1.0)
		action->Shield();
	if (network_outputs[OUT_JUMP] == 1.0)
		action->Jump();
	if (network_outputs[OUT_SPECIAL] == 1.0)
		action->Special();

	return action;
}

std::shared_ptr<std::vector<float>> AiEngine::get_input_layer(StateSharedPtr state) {
	std::shared_ptr<std::vector<float>> ret = state->get_buttons();
	std::shared_ptr<std::vector<float>> tmp = state->get_locations();
	ret->insert(ret->end(), tmp->begin(), tmp->end());
	tmp = state->get_damages();
	ret->insert(ret->end(), tmp->begin(), tmp->end());
	tmp = state->get_velocities();
	ret->insert(ret->end(), tmp->begin(), tmp->end());
	tmp = state->get_accelerations();
	ret->insert(ret->end(), tmp->begin(), tmp->end());
	tmp = state->get_player_distance();
	ret->insert(ret->end(), tmp->begin(), tmp->end());
	clamp(ret, 0.0, 1.0);
	// TODO: assert that ret.size() == POSSIBLE_STATE_COUNT
	return ret;
}

ActionSharedPtr AiEngine::predict(const StateSharedPtr state)
{
	// Generate input layer from state
	std::shared_ptr<std::vector<float>> input_layer = get_input_layer(state);

	// With our pump primed with 'hli', we now compute all of the hidden_layer_outputs
	std::vector<float> *hli = input_layer.get();
	for (int i = 0; i < this->hidden_layer_count; ++i)
	{
		compute_output_layer(this->hidden_layers[i], hli, this->hidden_layer_outputs[i]);
		hli = this->hidden_layer_outputs[i];
	}

	// Now we finally run our output layer (notice that hli was advanced by the for loop above)
	compute_output_layer(output_layer, hli, &output_layer_outputs);

	// Generate the action from the final output layer
	ActionSharedPtr a = get_action(output_layer_outputs);
	return a;
}

void AiEngine::adjustWeights(const ExperienceSharedPtr exp)
{
	// TODO
}

// TODO: In our code we assume that the input size will be greater than or equal to the layer size.
void NetworkLayer::init(unsigned layer_size, unsigned input_size)
{
	biases.resize(layer_size, 0.0);
	for (unsigned i = 0; i < layer_size; ++i)
	{
		weights.push_back(new std::vector<float>(input_size, 0.0));
	}
}

void NetworkLayer::rand()
{
	// "recommend scaling by the inverse of the square root of the fan-in"
	// https://stats.stackexchange.com/a/186351
	// https://arxiv.org/abs/1206.5533

	//initialization w = U([0,n]) * sqrt(2.0/n) where n is the number of inputs of your NN.
	// https://stats.stackexchange.com/a/248040
	// https://arxiv.org/abs/1502.01852

	float scale = sqrt(2.0 / this->input_size());
	for (auto perceptron_weights : this->weights)
	{
		for (auto weight = perceptron_weights->begin(); weight != perceptron_weights->end(); weight++)
		{
			(*weight) = scale * uniform_random(1, this->input_size());
		}
	}
	for (unsigned i = 0; i < this->biases.size(); ++i)
	{
		// Bias starts in the realm of "3 active inputs" -- seems like a good bias
		this->biases[i] = 8 * scale * uniform_random(1, this->input_size());
	}
}

std::vector<float>* NetworkLayer::getPerceptronWeights(unsigned i) const
{
	return weights[i];
}

const float * NetworkLayer::getPerceptronBias(unsigned i) const
{
	return &(biases[i]);
}

unsigned NetworkLayer::size() const
{
	return biases.size();
}

unsigned NetworkLayer::input_size() const
{
	return weights[0]->size();
}
