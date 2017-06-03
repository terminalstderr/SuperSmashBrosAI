#include "stdafx.h"
#include "ReplayMemory.h"


ReplayMemory::ReplayMemory()
{
	rng.seed(rng.default_seed);
}


ReplayMemory::~ReplayMemory()
{
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
		return memory[i];
	}
}

void ReplayMemory::seed(unsigned n)
{
	rng.seed(n);
}
