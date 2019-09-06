#pragma once

#include "Basement/Renderer/Shader.h"

#include <unordered_map>

#include <glm/glm.hpp>
#include <glad/glad.h>


namespace Basement {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& path);
		OpenGLShader(const std::string& vertexSource, const std::string& fragSource);
		virtual ~OpenGLShader() override;

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

		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string>PreprocessSource(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		
		void CheckShaderCompilation(GLint isCompiled, GLuint shader) const;
		void CheckProgramLinking(GLint isLinked, uint32_t program, const std::vector<GLuint>& shaderIds) const;
	private:
		uint32_t m_ProgramID = 0;
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
	};

}