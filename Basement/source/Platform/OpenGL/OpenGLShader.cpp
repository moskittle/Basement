#include "bmpch.h"
#include "OpenGLShader.h"
#include "OpenGLDebug.h"

#include <fstream>

#include <glm/gtc/type_ptr.hpp>

namespace Basement {
	
	static GLenum StringToShaderType(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		BM_CORE_ASSERT(false, "Unknown Shader Type: {0}!", type);
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string sourceCode = ReadFile(path);
		
		auto shaderSources = PreprocessSource(sourceCode);
		
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertSource, const std::string& fragSource)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		
		shaderSources[GL_VERTEX_SHADER] = vertSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragSource;
		
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ProgramID);
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::string source;
		std::ifstream input(path, std::ios::in, std::ios::binary);
		if (input)
		{
			input.seekg(0, std::ios::end);
			source.resize(input.tellg());
			input.seekg(0, std::ios::beg);
			input.read(&source[0], source.size());
			input.close();
		}
		else
		{
			BM_CORE_ERROR("Failed to open file '{0}'", path);
		}
		return source;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreprocessSource(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		
		std::string token = "#type";
		size_t tokenLength = token.size();

		size_t begin, end;
		size_t pos = source.find(token);

		while (pos != std::string::npos)
		{
			// Get shader type
			begin = pos + tokenLength + 1;
			end = source.find_first_of("\n", begin);
			BM_CORE_ASSERT(end != std::string::npos, "Syntax Error!");
			std::string shaderTypeStr = source.substr(begin, end - begin);
			GLenum shaderType = StringToShaderType(shaderTypeStr);

			// Get shader source
			begin = source.find_first_not_of("\n", end);
			end = source.find(token, begin);
			shaderSources[shaderType] = source.substr(begin, end - begin);

			pos = end;
			//BM_CORE_TRACE("{0} shader\n---------------------\n{1}\n---------------------", shaderTypeStr, shaderSources[shaderType]);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Create an empty program
		GLuint program = glCreateProgram();
		std::vector<GLuint> shaderIds; shaderIds.reserve(shaderSources.size());

		for (auto&& [shaderType, shaderSource] : shaderSources)		// Note: auto&& [first, second] supported by C++17
		{
			// Create an shader object
			GLuint shader = glCreateShader(shaderType);

			// Send source code to GL
			const GLchar* source = static_cast<const GLchar*>(shaderSource.c_str());
			glShaderSource(shader, 1, &source, 0);

			// Compile shader
			glCompileShader(shader);

			// Shader error handling
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			CheckShaderCompilation(isCompiled, shader);

			// Attach shaders to program
			glAttachShader(program, shader);

			shaderIds.push_back(shader);
		}

		// Link program
		glLinkProgram(program);

		// Program error handling
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		CheckProgramLinking(isLinked, program, shaderIds);

		// Detach shaders
		for (auto& shaderId : shaderIds)
		{
			glDetachShader(program, shaderId);
		}

		m_ProgramID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::CheckShaderCompilation(GLint isCompiled, GLuint shader) const
	{
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 200;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(shader);

			BM_CORE_ERROR("{0}", errorLog.data());
			BM_CORE_ASSERT(false, "Failed to compile vertex shader!");
			return;
		}
	}

	void OpenGLShader::CheckProgramLinking(GLint isLinked, uint32_t program, const std::vector<GLuint>& shaderIds) const
	{
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 200;
			glGetProgramiv(program, maxLength, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

			for (auto& shaderId : shaderIds)
			{
				glDeleteShader(shaderId);
			}

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
		OpenGLCall(glUniform3f(location, value.x, value.y, value.z));	// Unsolved error: 502

		//GLenum err;
		//while ((err = glGetError()) != GL_NO_ERROR) { std::cout << "Error: " << std::hex << err << std::endl; }
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
		BM_CORE_ASSERT(location != -1, "Invalid Uniform location!");
		m_UniformLocationCache[name] = location;
		return location;
	}

}