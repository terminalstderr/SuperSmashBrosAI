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

// Assumes that the vector is normalized
uint8_t Vector2::discrete_x()
{
	return static_cast<uint8_t> (this->x * 255);
}

uint8_t Vector2::discrete_y()
{
	return static_cast<uint8_t> (this->y * 255);
}


