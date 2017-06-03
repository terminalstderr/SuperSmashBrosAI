#pragma once
#include <memory>
#include "State.h"
#include "Action.h"


class Experience
{
private:
	State s;
	Action a;
	double r;
	State s2;
public:
	Experience(StateSharedPtr s, ActionSharedPtr a, double reward, StateSharedPtr s2);
};

typedef std::shared_ptr<Experience> ExperienceSharedPtr;

