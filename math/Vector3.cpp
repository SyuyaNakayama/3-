#include "Vector3.h"
#include <math.h>

Vector3::Vector3()
{
	x = 0.0f, y = 0.0f, z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::length() const
{
	return sqrtf(x * x + y * y + z * z);
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3& v) const
{
	Vector3 temp;

	temp =
	{
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	};

	return temp;
}

Vector3 Vector3::abs()
{
	Vector3 ans = *this;

	ans.x = fabsf(ans.x);
	ans.y = fabsf(ans.y);
	ans.z = fabsf(ans.z);

	return ans;
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

bool Vector3::operator<=(Vector3 vec)
{
	if (x <= vec.x && y <= vec.y && z <= vec.z) { return true; }
	return false;
}

bool Vector3::operator>=(Vector3 vec)
{
	if (x >= vec.x && y >= vec.y && z >= vec.z) { return true; }
	return false;
}

bool Vector3::operator<(Vector3 vec)
{
	if (x < vec.x && y < vec.y && z < vec.z) { return true; }
	return false;
}

bool Vector3::operator>(Vector3 vec)
{
	if (x > vec.x && y > vec.y && z > vec.z) { return true; }
	return false;
}

bool Vector3::operator==(Vector3 vec)
{
	if (x == vec.x && y == vec.y && z == vec.z) { return true; }
	return false;
}