#include "bmpch.h"

#include "Bone.h"
#include "Basement/Renderer/RendererUtil.h"

namespace Basement
{
	Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel)
	{
		m_Name = name;
		m_Id = id;
		m_LocalTransform = glm::mat4(1.0f);

		for (u32 i = 0; i < channel->mNumPositionKeys; ++i)
		{
			glm::vec3 position = RendererUtil::ConvertAssimpToGlmVector3(channel->mPositionKeys[i].mValue);
			float timeStamp = static_cast<float>(channel->mPositionKeys[i].mTime);
			m_Positions.push_back(KeyPosition(position, timeStamp));
		}

		for (u32 i = 0; i < channel->mNumRotationKeys; ++i)
		{
			aiQuaternion aiRotation = channel->mRotationKeys[i].mValue;
			glm::quat rotation = RendererUtil::ConvertAssimpToGlmQuaternion(aiRotation);
			float timeStamp = static_cast<float>(channel->mRotationKeys[i].mTime);
			m_Rotations.push_back(KeyRotation(rotation, timeStamp));
		}

		for (unsigned int i = 0; i < channel->mNumScalingKeys; ++i)
		{
			aiVector3D scale3d = channel->mScalingKeys[i].mValue;

			float scale1d = (scale3d.x + scale3d.y + scale3d.z) / 3.0f;
			float timeStamp = static_cast<float>(channel->mScalingKeys[i].mTime);
			m_Scales.push_back(KeyScale(scale1d, timeStamp));
		}
	}
	void Bone::Update(float animationTime)
	{
		glm::mat4 translation = InterpolatePosition(animationTime);
		glm::mat4 rotation = InterpolateRotation(animationTime);
		glm::mat4 scale = InterpolateScale(animationTime);

		//m_LocalTransform = Vqs(position, rotation, scale);
		m_LocalTransform = translation * rotation * scale;
	}

	// Linear interpolation
	glm::mat4 Bone::InterpolatePosition(float animationTime)
	{
		if (static_cast<int>(m_Positions.size()) == 1)
		{
			return glm::translate(glm::mat4(1.0f), m_Positions[0].Position);
		}

		int p0Index = GetPositionIndex(animationTime);
		int p1Index = p0Index + 1;
		float t = CalcInterpolationTValue(m_Positions[p0Index].TimeStamp, m_Positions[p1Index].TimeStamp, animationTime);

		glm::vec3 finalPosition = (1 - t) * m_Positions[p0Index].Position + t * m_Positions[p1Index].Position;
		return glm::translate(glm::mat4(1.0f), finalPosition);
	}

	// Spheric linear interpolation
	glm::mat4 Bone::InterpolateRotation(float animationTime)
	{
		if (static_cast<int>(m_Rotations.size()) == 1)
		{
			return glm::toMat4(glm::normalize(m_Rotations[0].Rotation));
		}

		int p0Index = GetRotationIndex(animationTime);
		int p1Index = p0Index + 1;
		//glm::quat rotation0 = (m_Rotations[p0Index].Rotation.Normalize());
		//glm::quat rotation1 = (m_Rotations[p1Index].Rotation.Normalize());
		glm::quat rotation0 = glm::normalize(m_Rotations[p0Index].Rotation);
		glm::quat rotation1 = glm::normalize(m_Rotations[p1Index].Rotation);
		float t = CalcInterpolationTValue(m_Rotations[p0Index].TimeStamp, m_Rotations[p1Index].TimeStamp, animationTime);

		glm::quat finalRotation = glm::slerp(rotation0, rotation1, t);
		return glm::toMat4(glm::normalize(finalRotation));

		////float rotation0DotRoation1 = rotation0.Dot(rotation1);
		//float rotation0DotRoation1 = glm::dot(rotation0, rotation1);

		//if (rotation0DotRoation1 < 0.0f)
		//{
		//	rotation0DotRoation1 *= -1.0f;
		//	rotation1 = rotation1 * -1.0f;
		//}

		//glm::quat finalRotation;
		//if (rotation0DotRoation1 > 1.0f - std::numeric_limits<float>::epsilon())
		//{
		//	finalRotation = rotation0 * (1 - t) + rotation1 * t;

		//	return glm::normalize(finalRotation);
		//}
		//else
		//{
		//	float theta = acos(rotation0DotRoation1);
		//	float rotation0Scale = sin((1.0f - t) * theta) / sin(theta);
		//	float rotation1Scale = sin(t * theta) / sin(theta);
		//	finalRotation = rotation0 * rotation0Scale + rotation1 * rotation1Scale;

		//	return glm::normalize(finalRotation);
		//}
	}

	// Linear interpolation
	glm::mat4 Bone::InterpolateScale(float animationTime)
	{
		if (static_cast<int>(m_Scales.size()) == 1)
		{
			auto scale = m_Scales[0].Scale;
			return glm::scale(glm::mat4(1.0f), glm::vec3(scale));
		}

		int p0Index = GetRotationIndex(animationTime);
		int p1Index = p0Index + 1;
		float t = CalcInterpolationTValue(m_Scales[p0Index].TimeStamp, m_Scales[p1Index].TimeStamp, animationTime);
		float finalScale = (1 - t) * m_Scales[p0Index].Scale + t * m_Scales[p1Index].Scale;

		return  glm::scale(glm::mat4(1.0f), glm::vec3(finalScale));;
	}

	int Bone::GetPositionIndex(float animationTime)
	{
		for (int i = 0; i < static_cast<int>(m_Positions.size()) - 1; ++i)
		{
			if (animationTime < m_Positions[i + 1].TimeStamp)
				return i;
		}
		BM_CORE_ASSERT("Invalid position index.", 0);

		return -1;
	}

	int Bone::GetRotationIndex(float animationTime)
	{
		for (int i = 0; i < static_cast<int>(m_Rotations.size()) - 1; ++i)
		{
			if (animationTime < m_Rotations[i + 1].TimeStamp)
				return i;
		}
		BM_CORE_ASSERT("Invalid rotation index.", 0);

		return -1;
	}

	int Bone::GetScaleIndex(float animationTime)
	{
		for (int i = 0; i < static_cast<int>(m_Scales.size()) - 1; ++i)
		{
			if (animationTime < m_Scales[i + 1].TimeStamp)
				return i;
		}
		BM_CORE_ASSERT("Invalid scale index.", 0);

		return -1;
	}

	float Bone::CalcInterpolationTValue(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;

		return midWayLength / framesDiff;
	}
}