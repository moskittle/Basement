#pragma once

#include <string>

namespace Basement {

	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragSource);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID = 0;
	};

}