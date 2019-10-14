#include "bmpch.h"

#include "Renderer.h"
#include "FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Basement {

	SharedPtr<FrameBuffer> FrameBuffer::Create(u32 width, u32 height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is currently supported")
					return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return std::make_shared<OpenGLFrameBuffer>(width, height);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI");
				return nullptr;
		}
	}

}