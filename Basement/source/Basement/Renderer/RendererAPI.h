#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Basement {

	class RendererAPI
	{
	public:
		enum class EAPI
		{
			None = 0, OpenGL = 1
		};

		struct RendererAPIInfo
		{
			std::string Vendor;
			std::string Renderer;
			std::string Version;

			int MaxSamples;
			float MaxAnisotropy;
		};

	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void EnableDepthTest() = 0;
		virtual void DrawIndex(const Shared<VertexArray>& vertexArray) = 0;
		virtual void DrawArrays(const Shared<VertexArray>& vertexArray, uint32_t first, uint32_t count) = 0;

		static EAPI GetRendererAPI() { return s_Instance; };
		static RendererAPIInfo& GetRendererAPIInfo() { static RendererAPIInfo info; return info; }
	protected:
		RendererAPI() = default;
	private:
		RendererAPI(const RendererAPI&) = delete;
		RendererAPI& operator=(const RendererAPI&) = delete;
	private:
		static EAPI s_Instance;
	};


}