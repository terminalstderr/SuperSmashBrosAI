#pragma once
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
	Experience(State &s, Action &a, double reward, State &s2);
};

