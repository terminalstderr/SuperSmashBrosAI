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
}

void compute_output_layer(const NetworkLayer layer, const std::vector<float> *input_layer, std::vector<float> *output_layer)
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

ActionSharedPtr AiEngine::predict(const StateSharedPtr state)
{
	// Need to generalize this to be all hidden layers:
	compute_output_layer();
	// TODO
	ActionSharedPtr a = ActionSharedPtr(new Action());
	a->Attack();
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
