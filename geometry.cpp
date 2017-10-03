#include "global.h"

Vector3::Vector3()
{
	e[0] = 0.0f;
	e[1] = 0.0f;
	e[2] = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
	e[0] = x;
	e[1] = y;
	e[2] = z;
}


float & Vector3::x()
{
	return e[0];
}
float & Vector3::y()
{
	return e[1];
}
float & Vector3::z()
{
	return e[2];
}

float Vector3::x() const
{
	return e[0];
}
float Vector3::y() const
{
	return e[1];
}
float Vector3::z() const
{
	return e[2];
}


const float * Vector3::elements() const
{
	return e;
}


Vector3 Vector3::operator+(const Vector3 & v) const
{
	return Vector3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
}

Vector3 Vector3::operator-(const Vector3 & v) const
{
	return Vector3(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-e[0], -e[1], -e[2]);
}

float Vector3::operator*(const Vector3 & v) const
{
	return (e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2]);
}

Vector3 Vector3::operator^(const Vector3 & v) const
{
	return Vector3(e[1] * v.e[2] - e[2] * v.e[1], e[2] * v.e[0] - e[0] * v.e[2], e[0] * v.e[1] - e[1] * v.e[0]);
}

Vector3 Vector3::operator*(float s) const
{
	return Vector3(e[0] * s, e[1] * s, e[2] * s);
}

Vector3 Vector3::operator/(float s) const
{
	return Vector3(e[0] / s, e[1] / s, e[2] / s);
}

Vector3 & Vector3::operator+=(const Vector3 & v)
{
	return (*this = *this + v);
}

Vector3 & Vector3::operator-=(const Vector3 & v)
{
	return (*this = *this - v);
}

Vector3 & Vector3::operator*=(float s)
{
	return (*this = *this * s);
}

Vector3 & Vector3::operator/=(float s)
{
	return (*this = *this / s);
}


float Vector3::length() const
{
	return sqrt(*this * *this);
}

Vector3 Vector3::normalize() const
{
	return *this / length();
}


Vector3 Vector3::rotateX(float rad) const
{
	return Vector3(e[0], e[1] * cos(rad) - e[2] * sin(rad), e[2] * cos(rad) + e[1] * sin(rad));
}
Vector3 Vector3::rotateY(float rad) const
{
	return Vector3(e[0] * cos(rad) + e[2] * sin(rad), e[1], e[2] * cos(rad) - e[0] * sin(rad));
}
Vector3 Vector3::rotateZ(float rad) const
{
	return Vector3(e[0] * cos(rad) - e[1] * sin(rad), e[1] * cos(rad) + e[0] * sin(rad), e[2]);
}
Vector3 Vector3::rotate(const Vector3 & eulerxyz) const
{
	return this->rotateX(eulerxyz.e[0]).rotateY(eulerxyz.e[1]).rotateZ(eulerxyz.e[2]);
}
