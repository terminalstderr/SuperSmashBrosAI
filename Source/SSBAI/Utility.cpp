#include "stdafx.h"
#include "Utility.h"

Vector2::Vector2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

void Vector2::update(float x, float y)
{
	this->x = x;
	this->y = y;
}
