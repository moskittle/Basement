#include "bmpch.h"

#include "Quaternion.h"

namespace Basement
{


	float Quaternion::Length()
	{
		return sqrt(s * s + glm::dot(v, v));
	}

	Quaternion Quaternion::Normalize()
	{
		Quaternion result;
		float invLength = 1.0f / this->Length();
		result.v = this->v * invLength;
		result.s = this->s * invLength;

		return result;
	}

	Quaternion Quaternion::Inverse()
	{
		Quaternion conjugateQuat(-v, s);

		return conjugateQuat.Normalize();
	}

	float Quaternion::Dot(const Quaternion& rhs)
	{
		return this->s * rhs.s + glm::dot(this->v, rhs.v);
	}

	Quaternion& Quaternion::operator=(const Quaternion& rhs)
	{
		this->v = rhs.v;
		this->s = rhs.s;

		return *this;
	}

	Quaternion Quaternion::operator+(const Quaternion& rhs)
	{
		Quaternion result;
		result.v = this->v + rhs.v;
		result.s = this->s + rhs.s;

		return result;
	}

	Quaternion Quaternion::operator*(const float& scale)
	{
		Quaternion result;
		result.v = this->v * scale;
		result.s = this->s * scale;

		return *this;
	}

	Quaternion Quaternion::operator*(const glm::vec3& _vec)
	{
		return (*this) * Quaternion(_vec, 0.0f);
	}

	Quaternion Quaternion::operator*(const Quaternion& rhs)
	{
		Quaternion result;

		result.v = this->s * rhs.v + this->v * rhs.s + glm::cross(this->v, rhs.v);
		result.s = this->s * rhs.s - glm::dot(this->v, rhs.v);

		return result;
	}
}