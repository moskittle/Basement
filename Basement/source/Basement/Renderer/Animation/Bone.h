#pragma once

#include "Vqs.h"
#include "Basement/Renderer/Buffer.h"

#include <glm/glm.hpp>

namespace Basement
{

	struct Bone
	{
		int Id;
		Vqs OffsetVqs;	// an offset matrix for bones

		Vertex JointVertex;
		int IndexInList;
	};

}