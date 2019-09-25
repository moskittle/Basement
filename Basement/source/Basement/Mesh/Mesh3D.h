#pragma once

#include <glm/glm.hpp>

namespace Basement {

	class Mesh3D
	{
	public:
		struct Face {
			uint32_t index[3];
			uint32_t operator[](int i) const { return index[i]; }
			uint32_t& operator[](int i) { return index[i]; }
		};
	public:
		virtual ~Mesh3D(void) = default;
		virtual uint32_t			GetVertexCount() = 0;
		virtual float*				GetVertexArray() = 0;
		virtual float*				GetNormalArray() = 0;
		virtual uint32_t			GetFaceCount() = 0;
		virtual uint32_t*			GetFaceArray() = 0;
	};

}