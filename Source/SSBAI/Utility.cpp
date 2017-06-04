#include "stdafx.h"
#include "Utility.h"
#include <memory>
#include <vector>
#include <algorithm>


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

Vector2 Vector2::operator-(const Vector2 & rhs)
{
	Vector2 ret = Vector2();
	ret.update(this->x - rhs.x, this->y - rhs.y);
	return ret;
}

Vector2 Vector2::operator/(const double rhs)
{
	Vector2 ret = Vector2();
	ret.update(this->x/rhs, this->y/rhs);
	return ret;
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

void clamp(std::shared_ptr<std::vector<float>> a, float min_clamp, float max_clamp)
{
	for (std::vector<float>::iterator val = a->begin(); val != a->end(); ++val) {
		*val = (std::max)(min_clamp, (std::min)(*val, max_clamp));
	}
}
