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
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), this->v);
		glm::mat4 rotation = glm::toMat4(glm::normalize(glm::quat(this->q.s, this->q.v)));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(this->s));

		return translation * rotation * scale;
	}

	Vqs Vqs::Inverse()
	{
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

	// quat * vec = q * (vec * s) * q^(-1) + v
	glm::vec3 Vqs::operator*(const glm::vec3& vec)
	{
		return (this->q * (vec * this->s) * this->q.Inverse()).v + this->v;
	}

}