#include "bmpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Basement {

	ShaderProgram* ShaderProgram::Create(const std::string& vertexSource, const std::string& fragSource)
	{
		switch (RendererAPI::GetRendererAPI())
		{
		case RendererAPI::EAPI::None:
			BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
		case RendererAPI::EAPI::OpenGL:
			return new OpenGLShaderProgram(vertexSource, fragSource);
		default:
			BM_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
	}


}