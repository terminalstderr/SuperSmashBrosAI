#pragma once
#include <vector>
#include <memory>
#include "Experience.h"
#include <random>

// We have decided for performance reasons that the Replay memory will simply be a containers for pointers to experiences.
// The actual experience data structures will be malloc/new elsewhere then passed to this container.
class ReplayMemory
{
private:
	std::vector<ExperienceSharedPtr> memory;
	std::mt19937_64 rng;

public:
	ReplayMemory();
	~ReplayMemory();
	void addExperience(ExperienceSharedPtr e);
	/*
	Gets a random experience from the replay memory.
	*/
	ExperienceSharedPtr sampleExperience();
};