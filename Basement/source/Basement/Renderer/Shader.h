#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Basement {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vertexSource, const std::string& fragSource);
	private:
		uint32_t m_ProgramID = 0;
	};

}