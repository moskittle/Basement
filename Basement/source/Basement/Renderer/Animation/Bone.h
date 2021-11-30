#pragma once

#include "Basement/Renderer/Buffer.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/scene.h>

namespace Basement
{

	struct BoneData
	{
		int Id;
		glm::mat4 offsetMatrix;	// offset to keyframe
		Vertex JointVertex;
		int IndexInList;
	};

	struct KeyPosition
	{
		glm::vec3 Position;
		float TimeStamp;

		KeyPosition(const glm::vec3& position, float timeStamp) : Position(position), TimeStamp(timeStamp) {}
	};

	struct KeyRotation
	{
		glm::quat Rotation;
		float TimeStamp;

		KeyRotation(const glm::quat& rotation, float timeStamp) : Rotation(rotation), TimeStamp(timeStamp) {}
	};

	struct KeyScale
	{
		float Scale;
		float TimeStamp;

		KeyScale(float scale, float timeStamp) : Scale(scale), TimeStamp(timeStamp) {}
	};

	class Bone
	{
	public:
		Bone() = default;
		Bone(const std::string& name, int id, const aiNodeAnim* channel);

		void Update(float animationTime);

		glm::mat4 InterpolatePosition(float animationTime);
		glm::mat4 InterpolateRotation(float animationTime);
		glm::mat4 InterpolateScale(float animationTime);

		int GetPositionIndex(float animationTime);
		int GetRotationIndex(float animationTime);
		int GetScaleIndex(float animationTime);

		const std::string& GetName() { return m_Name; }
		const int& GetId() { return m_Id; }
		const glm::mat4& GetLocalTransformation() { return m_LocalTransform; }

	private:
		float CalcInterpolationTValue(float lastTimeStamp, float nextTimeStamp, float animationTime);

		std::string m_Name;
		int m_Id;
		glm::mat4 m_LocalTransform;

		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scales;
	};

}