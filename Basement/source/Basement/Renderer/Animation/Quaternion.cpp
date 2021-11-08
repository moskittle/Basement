#include "bmpch.h"

#include "Quaternion.h"

namespace Basement
{


	float Quaternion::Length()
	{
		return sqrt(s * s + glm::dot(v, v));
	}

	Quaternion& Quaternion::Normalize()
	{
		float invLength = 1.0f / this->Length();
		this->v *= invLength;
		this->s *= invLength;

		return *this;
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

	Quaternion& Quaternion::operator+(const Quaternion& rhs)
	{
		this->v += rhs.v;
		this->s += rhs.s;

		return *this;
	}

	Quaternion& Quaternion::operator*(const float& scale)
	{
		this->v *= scale;
		this->s *= scale;

		return *this;
	}

	Quaternion& Quaternion::operator*(const glm::vec3& _vec)
	{
		return *this * Quaternion(_vec, 0.0f);
	}

	Quaternion& Quaternion::operator*(const Quaternion& rhs)
	{
		this->v = this->s * rhs.v + this->v * rhs.s + glm::cross(this->v, rhs.v);
		this->s = this->s * rhs.s - glm::dot(this->v, rhs.v);

		return *this;
	}
}