#include "stdafx.h"
#include "ReplayMemory.h"


ReplayMemory::ReplayMemory()
{
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
		unsigned i = uniform_random(1, memory.size());
		return memory[i-1];
	}
}