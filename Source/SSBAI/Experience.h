#pragma once
#include <memory>
#include "State.h"
#include "Action.h"

typedef std::shared_ptr<Experience> ExperienceSharedPtr;

class Experience
{
private:
	State s;
	Action a;
	double r;
	State s2;
public:
	Experience(State &s, Action &a, double reward, State &s2);
};

