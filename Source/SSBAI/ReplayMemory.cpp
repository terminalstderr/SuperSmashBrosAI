#include "stdafx.h"
#include "ReplayMemory.h"


ReplayMemory::ReplayMemory()
{
	rng.seed(rng.default_seed);
}


ReplayMemory::~ReplayMemory()
{
}

void ReplayMemory::addExperience(ExperienceSharedPtr e)
{
	memory.push_back(e);
}

ExperienceSharedPtr ReplayMemory::sampleExperience()
{
	if (memory.empty()) 
	{
		// TODO
		return NULL;
	}
	else
	{
		std::uniform_int_distribution<unsigned> d(1, memory.size());
		unsigned i = d(rng);
		return memory[i-1];
	}
}

void ReplayMemory::seed(unsigned n)
{
	rng.seed(n);
}
