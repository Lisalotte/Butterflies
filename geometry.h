#ifndef GEOMETRY_H
#define GEOMETRY_H

class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);

	float & x();
	float & y();
	float & z();

	float x() const;
	float y() const;
	float z() const;

	const float * elements() const;

	Vector3 operator+(const Vector3 & v) const;
	Vector3 operator-(const Vector3 & v) const;
	Vector3 operator-() const;
	float operator*(const Vector3 & v) const;
	Vector3 operator^(const Vector3 & v) const;
	Vector3 operator*(float s) const;
	Vector3 operator/(float s) const;

	Vector3 & operator+=(const Vector3 & v);
	Vector3 & operator-=(const Vector3 & v);
	Vector3 & operator*=(float s);
	Vector3 & operator/=(float s);

	float length() const;
	Vector3 normalize() const;

	Vector3 rotateX(float rad) const;
	Vector3 rotateY(float rad) const;
	Vector3 rotateZ(float rad) const;
	Vector3 rotate(const Vector3 & eulerxyz) const;

private:
	float e[3];
};

#endif