#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Basement {

	class Model
	{
	public:
		Model(const std::string& filePath);
		~Model() = default;

		//void Draw();
	private:
		void Load(const std::string& filePath);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Shared<Texture2D>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
		
		Shared<Shader> m_Shader;
		//Shared<Material> m_Material;

		const aiScene* m_Scene;
		Unique<Assimp::Importer> m_Importer;

	};





}