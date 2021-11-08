#pragma once

#include "Mesh.h"
#include "Animation/Bone.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

namespace Basement {

	class Model
	{
	public:
		Model(const std::string& filePath);
		~Model() = default;

		static SharedPtr<Model> Create(const std::string& filePath);
		void Draw(SharedPtr<Shader> shader, const glm::mat4& model);
		void DrawOutline(SharedPtr<Shader> shader, const glm::mat4& model);
	private:
		void Load(const std::string& filePath);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void ProcessVertices(aiMesh* mesh, std::vector<Vertex>& vertices);
		void ProcessIndices(aiMesh* mesh, std::vector<u32>& indices);
		void ProcessMaterial(aiMesh* mesh, const aiScene* scene, MaterialAttrib& materialAttrib, std::vector<SharedPtr<Texture2D>>& textures);
		void ProcessBone(aiMesh* mesh, std::vector<Vertex>& vertices);
		std::vector<SharedPtr<Texture2D>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

		void ResetVertexBoneData(Vertex& vertex);
		void SetVertexBoneData(Vertex& vertex, int id, float weight);

		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		const aiScene* m_Scene;
		UniquePtr<Assimp::Importer> m_Importer;
		std::unordered_map<std::string, Bone> m_BoneMap;
	};

}