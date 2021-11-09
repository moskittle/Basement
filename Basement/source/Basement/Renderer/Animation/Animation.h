#pragma once

#include "../Model.h"
#include "../Buffer.h"
#include "Bone.h"
#include "Vqs.h"

namespace Basement
{

	struct BoneNode
	{
		std::string name;
		Vqs localTransformation;
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
		void StoreBone(const BoneNode* node, int parentBoneIndex);

		// getters & setters
		double GetDuration() { return m_Duration; }
		double GetTicksPerSecond() { return m_TicksPerSecond; }
		const std::vector<SharedPtr<Bone>>& GetBones() { return m_Bones; }
		const std::unordered_map<std::string, BoneData>& GetBoneDataMap() { return m_BoneDataMap; }
		const SharedPtr<BoneNode> GetRootNode() { return m_RootNode; }
	private:
		SharedPtr<BoneNode> ReadHierarchyData(const aiNode* source);
		void ReadMissingBones(const aiAnimation* animation, Model& model);

		float m_Duration;
		int m_TicksPerSecond;
		std::vector<SharedPtr<Bone>> m_Bones;
		SharedPtr<BoneNode> m_RootNode;
		std::unordered_map<std::string, BoneData> m_BoneDataMap;

		// Data for drawing bones and joints
		std::vector<Vertex> m_BoneVertices;
		std::vector<u32> m_JointIndices;
		std::vector<u32> m_BoneIndices;
	};

}