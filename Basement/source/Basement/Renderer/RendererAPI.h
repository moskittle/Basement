#pragma once

#include "VertexArray.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Basement {

	class RendererAPI
	{
	public:
		enum class EAPI
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndex(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static EAPI GetRendererAPI() { return s_Instance; };
	protected:
		RendererAPI() = default;
	private:
		RendererAPI(const RendererAPI&) = delete;
		RendererAPI& operator=(const RendererAPI&) = delete;
	private:
		static EAPI s_Instance;
	};


}