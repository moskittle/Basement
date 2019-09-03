#pragma once

#include "Basement/Renderer/Shader.h"

#include <unordered_map>

#include <glad/glad.h>


namespace Basement {

	class OpenGLShaderProgram : public ShaderProgram
	{
	public:
		OpenGLShaderProgram(const std::string& vertexSource, const std::string& fragSource);
		virtual ~OpenGLShaderProgram() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;


		// TODO: Replace std::string with const char*
		void UploadUniform1i(const std::string& name, const int& value);
		void UploadUniform1f(const std::string& name, const float& value);
		void UploadUniform2f(const std::string& name, const glm::vec2& value);
		void UploadUniform3f(const std::string& name, const glm::vec3& value);
		void UploadUniform4f(const std::string& name, const glm::vec4& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& value);
	private:
		GLint GetUniformLocation(const std::string& name) const;
	private:
		uint32_t m_ProgramID = 0;
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
	};

}