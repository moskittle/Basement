#include "bmpch.h"
#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Basement {

	OpenGLShaderProgram::OpenGLShaderProgram(const std::string& vertSource, const std::string& fragSource)
	{
		// Create an empty vertex shader object
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send vertex source code to GL
		const GLchar* source = static_cast<const GLchar*>(vertSource.c_str());
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile vertex shader
		glCompileShader(vertexShader);

		// Error handling
		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 50;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(vertexShader);

			BM_CORE_ERROR("{0}", errorLog.data());
			BM_CORE_ASSERT(false, "Failed to compile vertex shader!");

			return;
		}

		// Create an empty fragment shader object
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send fragment source code to GL
		source = static_cast<const GLchar*>(fragSource.c_str());
		glShaderSource(fragShader, 1, &source, 0);

		// Compile fragment shader
		glCompileShader(fragShader);


		// Error handling
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 50;
			glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(vertexShader);
			glDeleteShader(fragShader);

			BM_CORE_ERROR("{0}", errorLog.data());
			BM_CORE_ASSERT(false, "Failed to compile fragment shader!");
		}

		// Create an empty program
		m_ProgramID = glCreateProgram();
		GLuint& program = m_ProgramID;

		// Attach shaders to program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragShader);

		// Link program
		glLinkProgram(program);

		// Error handling
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 50;
			glGetProgramiv(program, maxLength, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(vertexShader);
			glDeleteShader(fragShader);
			glDeleteProgram(program);

			BM_CORE_ERROR("{0}", errorLog.data());
			BM_CORE_ASSERT(false, "Failed to link shader");
			return;
		}


	}

	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		glDeleteProgram(m_ProgramID);
	}

	void OpenGLShaderProgram::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void OpenGLShaderProgram::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShaderProgram::UploadUniform1i(const std::string& name, const int& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShaderProgram::UploadUniform1f(const std::string& name, const float& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShaderProgram::UploadUniform2f(const std::string& name, const glm::vec2& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShaderProgram::UploadUniform3f(const std::string& name, const glm::vec3& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShaderProgram::UploadUniform4f(const std::string& name, const glm::vec4& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShaderProgram::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		GLint location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShaderProgram::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = GetUniformLocation(name);
		BM_CORE_ASSERT((location != -1), "Invalid UniformMat4 location!");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	GLint OpenGLShaderProgram::GetUniformLocation(const std::string& name)
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