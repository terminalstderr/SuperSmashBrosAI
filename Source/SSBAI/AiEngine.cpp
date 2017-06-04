#include "stdafx.h"
#include "AiEngine.h"
#include "Utility.h"


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
		nl.init(hidden_layer_width, hidden_layer_width);
		hidden_layers.push_back(nl);
		std::vector<float> *hlo = new std::vector<float>(hidden_layer_width, 0.0);
		hidden_layer_outputs.push_back(hlo);
	}
	// Setup the meta variables for this data structure
	this->hidden_layer_count = hidden_layer_count;
	this->hidden_layer_width = hidden_layer_width;
}

// TODO: The outer for loop can be parallelized.
// TODO: why are we passing a copy of NetworkLayer instead of a pointer to it???
void AiEngine::compute_output_layer(const NetworkLayer layer, const std::vector<float> *input_layer, std::vector<float> *output_layer)
{
	// for every perceptron in the layer
	for (unsigned p = 0; p < layer.size(); ++p)
	{
		// compute the magnitude of this perceptron
		std::vector<float>	*weights = layer.getPerceptronWeights(p);
		const float			*bias = layer.getPerceptronBias(p);
		float accumulator = 0.0;
		for (unsigned i = 0; i < this->hidden_layer_width; ++i)
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
	// TODO
	action->Attack();
	return action;
}

std::shared_ptr<std::vector<float>> AiEngine::get_input_layer(StateSharedPtr state) {
	std::shared_ptr<std::vector<float>> ret = state->get_buttons();
	std::shared_ptr<std::vector<float>> tmp = state->get_locations();
	ret->insert(ret->end(), tmp->begin(), tmp->end());
	tmp = state->get_velocities();
	ret->insert(ret->end(), tmp->begin(), tmp->end());
	tmp = state->get_player_distance();
	ret->insert(ret->end(), tmp->begin(), tmp->end());
	// XXX We need to make sure that the input layer is at least the size of the first hidden network layer
	ret->resize(this->hidden_layer_width, 0.0);
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

void NetworkLayer::init(unsigned layer_size, unsigned input_size)
{
	biases.resize(layer_size, 0.0);
	weights.resize(layer_size, new std::vector<float>(input_size, 0.0));
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
