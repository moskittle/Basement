#include "bmpch.h"

#include "Vqs.h"
#include "Basement/Renderer/RendererUtil.h"

namespace Basement
{

	Vqs::Vqs(aiMatrix4x4 matrix) : v(), q(), s()
	{
		aiVector3D _v;
		aiQuaternion _q;
		aiVector3D _s;

		matrix.Decompose(_s, _q, _v);

		this->v = RendererUtil::ConvertAssimpToGlmVector3(_v);
		this->q = Quaternion(_q);
		this->s = _s.x;
	}

	glm::mat4 Vqs::ConvertToMatrix()
	{
		return glm::mat4();
	}

	Vqs Vqs::Inverse()
	{
		//glm::vec3 inversV = VQS(glm::vec3(0), q.GetInverseQuaternion(), 1.0f / s) * -v;
		//return VQS(inversV, q.Inverse(), 1.0f / s);

		glm::vec3 inverseV = Vqs(glm::vec3(0), q.Inverse(), 1.0f / s) * -v;

		return Vqs(inverseV, q.Inverse(), 1.0f / s);
	}

	Vqs& Vqs::operator*(const Vqs& rhs)
	{
		this->q = this->q * rhs.q;
		this->s = this->s * rhs.s;
		this->v = this->v * rhs.v;

		return *this;
	}

	glm::vec4 Vqs::operator*(const glm::vec3& vec)
	{
		return glm::vec4();
	}

}