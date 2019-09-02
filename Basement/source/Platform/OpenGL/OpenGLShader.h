#pragma once

#include "Basement/Renderer/Shader.h"

namespace Basement {

	class OpenGLShaderProgram : public ShaderProgram
	{
	public:
		OpenGLShaderProgram(const std::string& vertexSource, const std::string& fragSource);
		virtual ~OpenGLShaderProgram() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& name, const int& values);
		void UploadUniformFloat(const std::string& name, const float& values);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformMat3(const std::string& name, const glm::mat3& values);
		void UploadUniformMat4(const std::string& name, const glm::mat4& values);
	private:
		uint32_t m_ProgramID = 0;
	};

}