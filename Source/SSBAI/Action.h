#pragma once

#include "Utility.h"
#include <random>

class Action
{
private:
	// My 
	MYBUTTONS buttons;

public:
	Action();
	~Action();
	void apply(uint32_t *controller);
	// We have two options as far as I see -- we can have very discrete actions then we make a function for each -- i.e. void punch(), void kick(), etc...'
	// Alternately, we might just update ut
	void update();
};

