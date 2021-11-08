#pragma once

#include "Quaternion.h"

#include <glm/glm.hpp>
#include <assimp/scene.h>

namespace Basement
{

	struct Vqs
	{
	public:
		Vqs() = default;
		Vqs(glm::vec3 _v, Quaternion _q, float _s) : v(_v), q(_q), s(_s) {}
		Vqs(const Vqs& rhs) : v(rhs.v), q(rhs.q), s(rhs.s) {}
		Vqs(aiMatrix4x4 matrix);
		~Vqs() = default;

		glm::mat4 ConvertToMatrix();
		Vqs Inverse();

		glm::vec3 operator*(const glm::vec3& vec);
		Vqs& operator*(const Vqs& rhs);

	public:
		glm::vec3 v;	// translation
		Quaternion q;	// rotation
		float s;		// scale
	};

}