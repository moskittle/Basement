#pragma once

#include <glm/glm.hpp>

namespace Basement {

	class Mesh3D
	{
	public:
		struct Face {
			u32 index[3];
			u32 operator[](int i) const { return index[i]; }
			u32& operator[](int i) { return index[i]; }
		};
	public:
		virtual ~Mesh3D(void) = default;
		virtual u32			GetVertexCount() = 0;
		virtual float*				GetVertexArray() = 0;
		virtual float*				GetNormalArray() = 0;
		virtual u32			GetFaceCount() = 0;
		virtual u32*			GetFaceArray() = 0;
	};

}