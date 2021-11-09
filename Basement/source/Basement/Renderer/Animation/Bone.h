#pragma once

#include "Vqs.h"
#include "Basement/Renderer/Buffer.h"

#include <glm/glm.hpp>

namespace Basement
{

	struct BoneData
	{
		int Id;
		Vqs OffsetVqs;	// an offset matrix for bones

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
		Quaternion Rotation;
		float TimeStamp;

		KeyRotation(const Quaternion& rotation, float timeStamp) : Rotation(rotation), TimeStamp(timeStamp) {}
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

		glm::vec3 InterpolatePosition(float animationTime);
		Quaternion InterpolateRotation(float animationTime);
		float InterpolateScale(float animationTime);

		int GetPositionIndex(float animationTime);
		int GetRotationIndex(float animationTime);
		int GetScaleIndex(float animationTime);

		const std::string& GetName() { return m_Name; }
		const int& GetId() { return m_Id; }
		const Vqs& GetLocalTransformation() { return m_LocalTransform; }

	private:
		float CalcInterpolationTValue(float lastTimeStamp, float nextTimeStamp, float animationTime);

		std::string m_Name;
		int m_Id;
		Vqs m_LocalTransform;

		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scales;
	};

}