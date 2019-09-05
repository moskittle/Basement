#include "bmpch.h"
#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Basement {

	OpenGLShader::OpenGLShader(const std::string& vertSource, const std::string& fragSource)
	{
		// Create an empty vertex shader object
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

		// Send vertex source code to GL
		const GLchar* source = static_cast<const GLchar*>(vertSource.c_str());
		glShaderSource(vertShader, 1, &source, 0);

		// Compile vertex shader
		glCompileShader(vertShader);

		// Error handling
		GLint isCompiled = 0;
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
		CheckShaderError(isCompiled, vertShader);

		// Create an empty fragment shader object
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send fragment source code to GL
		source = static_cast<const GLchar*>(fragSource.c_str());
		glShaderSource(fragShader, 1, &source, 0);

		// Compile fragment shader
		glCompileShader(fragShader);

		// Error handling
		CheckShaderError(isCompiled, fragShader);

		// Create an empty program
		m_ProgramID = glCreateProgram();
		GLuint& program = m_ProgramID;

		// Attach shaders to program
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);

		// Link program
		glLinkProgram(program);

		// Error handling
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::CheckShaderError(GLint isCompiled, GLuint shader)
	{
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 50;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(shader);

			BM_CORE_ERROR("{0}", errorLog.data());
			BM_CORE_ASSERT(false, "Failed to compile vertex shader!");
			return;
		}
	}

	void CheckShaderProgramError(GLint isLinked, GLuint vertShader, GLuint fragShader, uint32_t program)
	{
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 50;
			glGetProgramiv(program, maxLength, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(vertShader);
			glDeleteShader(fragShader);
			glDeleteProgram(program);

			BM_CORE_ERROR("{0}", errorLog.data());
			BM_CORE_ASSERT(false, "Failed to link shader");
			return;
		}
	}

	void OpenGLShader::UploadUniform1i(const std::string& name, const int& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniform1f(const std::string& name, const float& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniform2f(const std::string& name, const glm::vec2& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniform3f(const std::string& name, const glm::vec3& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);	// Unsolved error: 502

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) { std::cout << "Error: " << std::hex << err << std::endl; }
	}

	void OpenGLShader::UploadUniform4f(const std::string& name, const glm::vec4& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		GLint location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	GLint OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		{
			return m_UniformLocationCache[name];
		}

		GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
		BM_CORE_ASSERT((location != -1), "Invalid Uniform location!");
		m_UniformLocationCache[name] = location;
		return location;
	}

}