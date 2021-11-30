#pragma once

#include "../Model.h"
#include "../Buffer.h"
#include "Bone.h"

namespace Basement
{

	struct BoneNode
	{
		std::string name;
		glm::mat4 localTransformation;
		SharedPtr<BoneNode> parent;
		std::vector<SharedPtr<BoneNode>> children;
	};

	class Animation
	{
	public:
		Animation() = default; Animation(const std::string& path, SharedPtr<Model> model);
		~Animation() = default;

		SharedPtr<Bone> FindBone(const std::string& name);

		// Store data for rendering joints and bones
		void StoreJoints();
		void StoreBones(SharedPtr<BoneNode> node, int parentBoneIndex);
		void SetupSkeletonVertexArray();
		void DrawSkeleton(SharedPtr<Shader> shader, glm::mat4 modelMatrix, bool drawJoints, bool drawBones);

		// getters & setters
		double GetDuration() { return m_Duration; }
		double GetTicksPerSecond() { return m_TicksPerSecond; }
		const std::vector<SharedPtr<Bone>>& GetBones() { return m_Bones; }
		const std::unordered_map<std::string, BoneData>& GetBoneDataMap() { return m_BoneDataMap; }
		const SharedPtr<BoneNode> GetRootNode() { return m_RootNode; }
		std::vector<SharedPtr<BoneNode>> GenerateInverseKinematicsData(std::string endEffectorName);

	private:
		SharedPtr<BoneNode> ReadHierarchyData(const aiNode* source, SharedPtr<BoneNode> parentNode);
		void ReadMissingBones(const aiAnimation* animation, Model& model);
		SharedPtr<BoneNode> FindBoneNode(const std::string name, SharedPtr<BoneNode> node);

		float m_Duration;
		int m_TicksPerSecond;
		std::vector<SharedPtr<Bone>> m_Bones;
		SharedPtr<BoneNode> m_RootNode;
		std::unordered_map<std::string, BoneData> m_BoneDataMap;

		// Data for drawing bones and joints
		std::vector<Vertex> m_JointVertices;
		std::vector<u32> m_JointIndices;
		SharedPtr<VertexArray> m_JointVao;
		SharedPtr<VertexBuffer> m_JointVbo;
		SharedPtr<IndexBuffer> m_JointIbo;

		std::vector<u32> m_BoneIndices;
		SharedPtr<VertexArray> m_BoneVao;
		SharedPtr<VertexBuffer> m_BoneVbo;
		SharedPtr<IndexBuffer> m_BoneIbo;
	};

}