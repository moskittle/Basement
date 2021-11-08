#pragma once

#include <glm/glm.hpp>
#include <assimp/quaternion.h>

namespace Basement
{

	struct Quaternion
	{
	public:
		Quaternion() = default;
		Quaternion(glm::vec3 _v, float _s) : v(_v), s(_s) {}
		Quaternion(const Quaternion& rhs) : v(rhs.v), s(rhs.s) {}
		Quaternion(const aiQuaternion& aiQuat) : v(glm::vec3(aiQuat.x, aiQuat.y, aiQuat.z)), s(aiQuat.w) {}
		~Quaternion() = default;

		float Length();
		Quaternion Normalize();
		Quaternion Inverse();
		float Dot(const Quaternion& rhs);

		Quaternion& operator=(const Quaternion& rhs);
		Quaternion& operator+(const Quaternion& rhs);
		Quaternion& operator*(const float& scale);
		Quaternion& operator*(const glm::vec3& _vec);
		Quaternion& operator*(const Quaternion& rhs);

	public:
		glm::vec3 v;
		float s;
	};

}