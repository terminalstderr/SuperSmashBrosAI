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
	output_layer.init(POSSIBLE_ACTION_SPACE, hidden_layer_width);
	output_layer_outputs.resize(POSSIBLE_ACTION_SPACE, 0.0);
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
	output_layer.rand1();
	// Handle hidden layers' perceptrons (Network Layers) and output vectors
	for (unsigned i = 0; i < hidden_layer_count; i++) {
		hidden_layers[i].rand2();
	}
}

void AiEngine::forward_propogation_thresh(const NetworkLayer &layer, const std::vector<float> *input_layer, std::vector<float> *output_layer)
{
	// for every perceptron in the layer
#pragma omp parallel for
	for (int p = 0; p < layer.size(); ++p)
	{
		// compute the magnitude of this perceptron
		std::vector<float>	*weights = layer.getPerceptronWeights(p);
		const float			*bias = layer.getPerceptronBias(p);
		float accumulator = 0.0;
		// XXX Parallelizing this loop causes significant slow-down -- this step is massively vectorizable.
		for (int i = 0; i < weights->size(); ++i)
		{
			accumulator += (*input_layer)[i] * (*weights)[i];
		}
		// threshold check
		(*output_layer)[p] = accumulator >(*bias) ? 1.0 : 0.0;
	}
}

void AiEngine::forward_propogation_relu(const NetworkLayer & layer, const std::vector<float>* input_layer, std::vector<float>* output_layer)
{
	// for every perceptron in the layer
#pragma omp parallel for
	for (int p = 0; p < layer.size(); ++p)
	{
		// compute the magnitude of this perceptron
		std::vector<float>	*weights = layer.getPerceptronWeights(p);
		const float			*bias = layer.getPerceptronBias(p);
		float accumulator = 0.0;
		// XXX Parallelizing this loop causes significant slow-down -- this step is massively vectorizable.
		for (int i = 0; i < weights->size(); ++i)
		{
			accumulator += (*input_layer)[i] * (*weights)[i];
		}
		// Leaky Rectified Linear Unit (ReLU)
		accumulator += *bias;
		(*output_layer)[p] = accumulator > 0 ? accumulator : 0.01 * accumulator;
	}

}

void AiEngine::forward_propogation_linear(const NetworkLayer & layer, const std::vector<float>* input_layer, std::vector<float>* output_layer)
{
	// for every perceptron in the layer
#pragma omp parallel for
	for (int p = 0; p < layer.size(); ++p)
	{
		// compute the magnitude of this perceptron
		std::vector<float>	*weights = layer.getPerceptronWeights(p);
		const float			*bias = layer.getPerceptronBias(p);
		float accumulator = 0.0;
		// XXX Parallelizing this loop causes significant slow-down -- this step is massively vectorizable.
		for (int i = 0; i < weights->size(); ++i)
		{
			accumulator += (*input_layer)[i] * (*weights)[i];
		}
		// Leaky Rectified Linear Unit (ReLU)
		accumulator += *bias;
		(*output_layer)[p] = accumulator;
	}
}

ActionSharedPtr AiEngine::get_action(std::vector<float> &network_outputs)
{
	// Get the max output from the network
	uint8_t max_action = 0;
	float max_qvalue = -std::numeric_limits<float>::infinity();
	for (int i = 0; i < POSSIBLE_ACTION_SPACE; i++)
	{
		if (max_qvalue < network_outputs[i]) {
			max_qvalue = network_outputs[i];
			max_action = i;
		}
	}

	// Copy the max_action into a union
	ACTIONS amap;
	amap.Value = max_action;

	// Build the action from the selected max action value
	ActionSharedPtr action = ActionSharedPtr(new Action());
	uint8_t lr = amap.LEFT_RIGHT;
	uint8_t ud = amap.UP_DOWN;
	uint8_t s = amap.SPEED;
	float x = (lr == 0 ? -1.0f : 1.0f) * (s == 0 ? 0.5 : 1.0f);
	float y = (ud == 0 ? -1.0f : 1.0f) * (s == 0 ? 0.5 : 1.0f);
	Vector2 movement;
	movement.update(x, y);
	action->Move(movement);

	if (amap.ATTACK == 1)
		action->Attack();
	if (amap.SHEILD == 1)
		action->Shield();
	if (amap.JUMP == 1)
		action->Jump();
	if (amap.SPECIAL == 1)
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
	normalize(ret);
	// TODO: assert that ret.size() == POSSIBLE_STATE_COUNT
	return ret;
}

ActionSharedPtr AiEngine::predict(const StateSharedPtr state)
{
	// Generate input layer from state
	std::shared_ptr<std::vector<float>> input_layer = get_input_layer(state);

	// With our pump primed with 'hli', we now compute all of the hidden_layer_outputs
	std::vector<float> *hli = input_layer.get();
	// XXX Parallelizing this loop would require doing a 'per-stage' pipelining.
	// #pragma omp parallel for
	for (int i = 0; i < this->hidden_layer_count; ++i)
	{
		forward_propogation_relu(this->hidden_layers[i], hli, this->hidden_layer_outputs[i]);
		hli = this->hidden_layer_outputs[i];
	}

	// Now we finally run our output layer (notice that hli was advanced by the for loop above)
	forward_propogation_linear(output_layer, hli, &output_layer_outputs);

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

void NetworkLayer::rand1()
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

void NetworkLayer::rand2()
{
	// "recommend scaling by the inverse of the square root of the fan-in"
	// https://stats.stackexchange.com/a/186351
	// https://arxiv.org/abs/1206.5533

	//initialization w = U([0,n]) * sqrt(2.0/n) where n is the number of inputs of your NN.
	// https://stats.stackexchange.com/a/248040
	// https://arxiv.org/abs/1502.01852

	for (auto perceptron_weights : this->weights)
	{
		for (auto weight = perceptron_weights->begin(); weight != perceptron_weights->end(); weight++)
		{
			(*weight) = uniform_random(0.0f, 1.0f);
		}
	}
	for (unsigned i = 0; i < this->biases.size(); ++i)
	{
		// Bias starts in the realm of "3 active inputs" -- seems like a good bias
		this->biases[i] = 8 * uniform_random(0.0f, 1.0f);
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
