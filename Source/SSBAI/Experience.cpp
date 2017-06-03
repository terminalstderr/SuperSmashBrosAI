#include "stdafx.h"
#include "Experience.h"

Experience::Experience(StateSharedPtr s, ActionSharedPtr a, double reward, StateSharedPtr s2)
{
	// Get a copy of each of the provided values and store it into this experience
	this->s = *s;
	this->a = *a;
	this->r = reward;
	this->s2 = *s2;
}
