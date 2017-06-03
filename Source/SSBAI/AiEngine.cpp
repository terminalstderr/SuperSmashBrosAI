#include "stdafx.h"
#include "AiEngine.h"
#include "Utility.h"


AiEngine::AiEngine()
{
}


AiEngine::~AiEngine()
{
	// TODO: for item in weights vector, delete
}

ActionSharedPtr AiEngine::predict(const StateSharedPtr state)
{
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

std::vector<float>* NetworkLayer::getPerceptronWeights(unsigned i)
{
	return weights[i];
}

float * NetworkLayer::getPerceptronBias(unsigned i)
{
	return &(biases[i]);
}
