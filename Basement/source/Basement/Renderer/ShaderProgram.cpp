#include "bmpch.h"
#include "ShaderProgram.h"

#include <glad/glad.h>

namespace Basement {

	ShaderProgram::ShaderProgram(const std::string& vertSource, const std::string& fragSource)
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

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ProgramID);
	}

	void ShaderProgram::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void ShaderProgram::Unbind() const
	{
		glUseProgram(0);
	}

}