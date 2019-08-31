#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Basement {

	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexSource, const std::string& fragSource);
		~ShaderProgram();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_ProgramID = 0;
	};

}