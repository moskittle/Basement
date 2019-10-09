#include "bmpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Basement {

	SharedPtr<Texture2D> Texture2D::Create(const std::string& path, bool isRepeated)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is nto supported!");
				return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return std::make_shared<OpenGLTexture2D>(path, isRepeated);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI");
				return nullptr;
		}
	}

}