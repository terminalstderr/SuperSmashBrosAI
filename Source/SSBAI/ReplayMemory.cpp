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
	std::uniform_int_distribution<unsigned> d(1,memory.size());
	unsigned i = d(rng);
	return memory[i];
}
