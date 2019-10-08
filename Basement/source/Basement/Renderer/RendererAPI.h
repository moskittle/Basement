#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Basement {

	namespace RendererGL {
		// Stencil mask
		static u32 EnableStencilMask = 0xFF;
		static u32 DisableStencilMask = 0x00;

		// GL enum
		static u32 Always = 0x0207;
		static u32 NotEqual = 0x0205;

		static u32 Keep = 0x1E00;
		static u32 Replace = 0x1E01;
	}

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
		virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;
		
		virtual void EnableStencilTest() = 0;
		virtual void SetStencilOp(u32 allFail, u32 depthFail, u32 allPass) = 0;
		virtual void SetStencilMask(u32 mask) = 0;
		virtual void SetStencilFunc(u32 func, i32 ref, u32 mask) = 0;
		
		virtual void DrawIndex(const SharedPtr<VertexArray>& vertexArray) = 0;
		virtual void DrawArrays(const SharedPtr<VertexArray>& vertexArray, u32 first, u32 count) = 0;

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