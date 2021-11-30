#include "bmpch.h"

#include "Animation.h"
#include "../Renderer.h"
#include "../RendererUtil.h"

#include <glm/gtx/string_cast.hpp>

namespace Basement
{

	Animation::Animation(const std::string& path, SharedPtr<Model> model)
	{
		Assimp::Importer importer;

		auto scene = importer.ReadFile(path, aiProcess_Triangulate);
		BM_CORE_ASSERT(scene->mRootNode, "Scene root node is nullptr");

		auto animation = scene->mAnimations[0];
		m_Duration = static_cast<float>(animation->mDuration);
		m_TicksPerSecond = static_cast<int>(animation->mTicksPerSecond);

		BM_CORE_INFO("Load animation from: {0} ticksPerSecond: {1}, duration: {2}", path, m_TicksPerSecond, m_Duration);

		m_RootNode = ReadHierarchyData(scene->mRootNode, nullptr);
		ReadMissingBones(animation, *model);

		// Debug setup
		StoreJoints();
		StoreBones(m_RootNode, 0);
		SetupSkeletonVertexArray();
	}

	SharedPtr<Bone> Animation::FindBone(const std::string& name)
	{
		for (auto itr = m_Bones.cbegin(); itr != m_Bones.cend(); ++itr)
		{
			auto bone = *itr;
			if (bone->GetName() == name)
			{
				return bone;
			}
		}

		return nullptr;
	}

	void Animation::StoreJoints()
	{
		int i = 0;

		for (auto& [name, boneData] : m_BoneDataMap)
		{
			boneData.IndexInList = i;
			m_JointVertices.push_back(boneData.JointVertex);
			m_JointIndices.push_back(i);
			i++;
		}
	}

	void Animation::StoreBones(SharedPtr<BoneNode> node, int parentBoneIndex)
	{
		int index = parentBoneIndex;
		std::string& nodeName = node->name;
		if (m_BoneDataMap.find(nodeName) != m_BoneDataMap.end())
		{
			index = m_BoneDataMap[nodeName].IndexInList;
		}

		if (index != parentBoneIndex)
		{
			m_BoneIndices.push_back(parentBoneIndex);
			m_BoneIndices.push_back(index);
		}

		for (size_t i = 0; i < node->children.size(); ++i)
		{
			StoreBones(node->children[i], index);
		}
	}

	void Animation::SetupSkeletonVertexArray()
	{
		// Joints Vao
		BufferLayout bufferLayout = {
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float3, "a_Normal" },
			{ EShaderDataType::Float2, "a_TexCoord" },
			{ EShaderDataType::Int4, "a_BoneIds"},
			{ EShaderDataType::Float4, "a_BoneWeights"}
		};

		m_JointVao = VertexArray::Create();
		m_JointVbo = VertexBuffer::Create(m_JointVertices);
		m_JointVbo->SetLayout(bufferLayout);
		m_JointVao->AddVertexBuffer(m_JointVbo);
		m_JointIbo = IndexBuffer::Create(m_JointIndices);
		m_JointVao->SetIndexBuffer(m_JointIbo);

		// Bones Vao
		m_BoneVao = VertexArray::Create();
		m_BoneVbo = VertexBuffer::Create(m_JointVertices);	// share vertices with joints
		m_BoneVbo->SetLayout(bufferLayout);
		m_BoneVao->AddVertexBuffer(m_BoneVbo);
		m_BoneIbo = IndexBuffer::Create(m_BoneIndices);
		m_BoneVao->SetIndexBuffer(m_BoneIbo);
	}

	void Animation::DrawSkeleton(SharedPtr<Shader> shader, glm::mat4 modelMatrix, bool drawJoints, bool drawBones)
	{
		//draw joints
		if (drawJoints)
		{
			u32 indexCount = static_cast<u32>(m_JointIndices.size());
			Renderer::SubmitArraysForPoints(shader, m_JointVao, m_JointIndices[0], indexCount, modelMatrix);
		}

		// draw boines
		if (drawBones)
		{
			Renderer::SubmitForLines(shader, m_BoneVao, modelMatrix);
		}
	}

	SharedPtr<BoneNode> Animation::ReadHierarchyData(const aiNode* source, SharedPtr<BoneNode> parentNode)
	{
		auto currentNode = std::make_shared<BoneNode>();
		BM_CORE_ASSERT(source, "Invalid source.");

		currentNode->name = source->mName.data;
		currentNode->localTransformation = RendererUtil::ConvertAssimpToGlmMatrix4(source->mTransformation);
		currentNode->parent = parentNode;
		for (unsigned int i = 0; i < source->mNumChildren; i++)
		{
			SharedPtr<BoneNode> child = ReadHierarchyData(source->mChildren[i], currentNode);
			currentNode->children.push_back(child);
		}

		return currentNode;
	}

	void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
	{
		auto boneInfoMap = model.GetBoneMap();
		auto boneCount = static_cast<int>(boneInfoMap.size());

		m_BoneDataMap = boneInfoMap;

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].Id = boneCount;
				boneCount++;
			}
			m_Bones.push_back(std::make_shared<Bone>(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].Id, channel));
		}
	}

	SharedPtr<BoneNode> Animation::FindBoneNode(const std::string nodeName, SharedPtr<BoneNode> node)
	{
		if (nodeName == node->name)
		{
			return node;
		}

		std::queue<SharedPtr<BoneNode>> boneNodeQueue({ m_RootNode });
		while (!boneNodeQueue.empty())
		{
			auto queueSize = boneNodeQueue.size();

			for (int i = 0; i < queueSize; ++i)
			{
				auto currentNode = boneNodeQueue.front();
				boneNodeQueue.pop();

				if (nodeName == currentNode->name)
				{
					return currentNode;
				}

				for (auto child : currentNode->children)
				{
					boneNodeQueue.push(child);
				}
			}
		}

		BM_CORE_INFO("Bone Node {0} is not found", nodeName);
		return nullptr;
	}

	/// <summary>
	/// This function retrives the info of end effectors from skeleton.
	/// </summary>
	/// <param name="endEffectorName">The name of the end effector.</param>
	/// <returns></returns>
	std::vector<SharedPtr<BoneNode>> Animation::GenerateInverseKinematicsData(std::string endEffectorName)
	{
		auto endEffector = FindBoneNode(endEffectorName, m_RootNode);

		std::vector<SharedPtr<BoneNode>> endEffectors;
		while (endEffector != nullptr)
		{
			if (m_BoneDataMap.find(endEffector->name) != m_BoneDataMap.end())
			{
				endEffectors.push_back(endEffector);
			}
			endEffector = endEffector->parent;
		}

		return endEffectors;
	}

}