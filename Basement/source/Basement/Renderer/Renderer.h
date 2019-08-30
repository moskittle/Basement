#pragma once

#include "RenderCommand.h"

namespace Basement {
	
	class Renderer 
	{
	public:		
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::EAPI GetAPI() { return RendererAPI::GetRendererAPI(); }
	};

}