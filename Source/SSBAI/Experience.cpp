#include "stdafx.h"
#include "Experience.h"

Experience::Experience(State & s, Action & a, double reward, State & s2)
{
	this->s = s;
	this->a = a;
	this->r = reward;
	this->s2 = s2;
}
