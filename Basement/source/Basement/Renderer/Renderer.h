#pragma once

namespace Basement {

	enum class ERendererAPI
	{
		None = 0, OpenGL = 1
	};
	
	class Renderer 
	{
	public:
		inline static ERendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static ERendererAPI s_RendererAPI;
	};


}