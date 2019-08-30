#include "bmpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Basement {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}