#pragma once

#include <string>

namespace Basement {

	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexSource, const std::string& fragSource);
		~ShaderProgram();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_ProgramID = 0;
	};

}