#include "Vec2.h"
#include <math.h>


Vec2::Vec2()
{}

Vec2::Vec2(float xin, float yin)
	:x(xin), y(yin)
{}

Vec2 Vec2::operator+ (const Vec2& rhs) const
{
	//TODO
	return Vec2(this->x + rhs.x, this->y + rhs.y);
}

bool Vec2::operator == (const Vec2& rhs) const 
{
	return(this->x == rhs.x && this->y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
	return(this->x != rhs.x && this->y != rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	return Vec2(rhs.x - this->x, rhs.y - this->y);
}

Vec2 Vec2::operator / (const Vec2& rhs) const
{
	return Vec2(this->x / rhs.x, this->y / rhs.y);
}

Vec2 Vec2::operator * (const Vec2& rhs) const
{
	return Vec2(this->x * rhs.x, this->y * rhs.y);
}

Vec2 Vec2::operator += (const Vec2& rhs) const
{
	Vec2 toReturn = Vec2(this->x + rhs.x, this->y + rhs.y);
	return toReturn;
}

Vec2 Vec2::operator -= (const Vec2& rhs) const
{
	Vec2 toReturn = Vec2(this->x - rhs.x, this->y - rhs.y);
	return toReturn;
}

Vec2 Vec2::operator *= (const Vec2& rhs) const
{
	Vec2 toReturn = Vec2(this->x * rhs.x, this->y * rhs.y);
	return toReturn;
}

Vec2 Vec2::operator /= (const Vec2& rhs) const
{
	Vec2 toReturn = Vec2(this->x / rhs.x, this->y / rhs.y);
	return toReturn;
}

float Vec2::dist(const Vec2& rhs) const
{
	// first get difference between this and rhs
	Vec2 diff = Vec2(this->x - rhs.x, this->y - rhs.y);
	// then get the actual distance
	return sqrtf(diff.x * diff.x + diff.y * diff.y);
}

Vec2 Vec2::normalize()
{
	// first get length
	float length = sqrtf(x * x + y * y);
	// then normalize
	return Vec2(x / length, y / length);
}