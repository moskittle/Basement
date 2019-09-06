#include "bmpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Basement {

	Shader* Shader::Create(const std::string& path)
	{
		switch (RendererAPI::GetRendererAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return new OpenGLShader(path);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI!");
				return nullptr;
		}
	}

	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragSource)
	{
		switch (RendererAPI::GetRendererAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
					return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return new OpenGLShader(vertexSource, fragSource);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI!");
				return nullptr;
		}
	}


}