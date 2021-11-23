#pragma once

#pragma once

#include <assimp/quaternion.h>
#include <assimp/matrix4x4.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Basement
{
	namespace RendererUtil
	{
		static glm::mat4 ConvertAssimpToGlmMatrix4(const aiMatrix4x4& aiMatrix)
		{
			glm::mat4 glmMatrix;

			// the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
			glmMatrix[0][0] = aiMatrix.a1; glmMatrix[1][0] = aiMatrix.a2; glmMatrix[2][0] = aiMatrix.a3; glmMatrix[3][0] = aiMatrix.a4;
			glmMatrix[0][1] = aiMatrix.b1; glmMatrix[1][1] = aiMatrix.b2; glmMatrix[2][1] = aiMatrix.b3; glmMatrix[3][1] = aiMatrix.b4;
			glmMatrix[0][2] = aiMatrix.c1; glmMatrix[1][2] = aiMatrix.c2; glmMatrix[2][2] = aiMatrix.c3; glmMatrix[3][2] = aiMatrix.c4;
			glmMatrix[0][3] = aiMatrix.d1; glmMatrix[1][3] = aiMatrix.d2; glmMatrix[2][3] = aiMatrix.d3; glmMatrix[3][3] = aiMatrix.d4;

			return glmMatrix;
		}

		static glm::vec3 ConvertAssimpToGlmVector3(const aiVector3D& aiVec)
		{
			return glm::vec3(aiVec.x, aiVec.y, aiVec.z);
		}

		static glm::quat ConvertAssimpToGlmQuaternion(const aiQuaternion& aiQuat)
		{
			return glm::quat(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
		}

		// newVec = q * (oldVec* s) * q^-1 + v
		static glm::vec3 MultiplyVqsByVec3(glm::vec3 v, glm::quat q, float s, glm::vec3 vec)
		{
			return q * (vec * s) * glm::inverse(q) + v;
		}

		static glm::vec3 CalculateLocalPosition(glm::vec3 position, glm::quat rotation, float scale)
		{
			return MultiplyVqsByVec3(glm::vec3(0.0f), glm::inverse(rotation), 1 / scale, -position);
		}
	}
}