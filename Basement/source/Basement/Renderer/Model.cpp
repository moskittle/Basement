#include "bmpch.h"

#include "Model.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <glm/glm.hpp>

namespace Basement {

	static const uint32_t s_MeshImportFlags =
		aiProcess_Triangulate | 
		aiProcess_FlipUVs;

	Model::Model(const std::string& filePath)
	{
		m_Importer = std::make_unique<Assimp::Importer>();

		Load(filePath);
	}

	void Model::Load(const std::string& filePath)
	{
		m_Scene = m_Importer->ReadFile(filePath, s_MeshImportFlags);

		if (!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode)
		{
			BM_CORE_ERROR("Failed to load {0}\n{1}", filePath, m_Importer->GetErrorString());
			return;
		}

		m_Directory = filePath.substr(0, filePath.find_last_of('/'));

		ProcessNode(m_Scene->mRootNode, m_Scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
		
		// recursion
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNode(node->mChildren[i], scene);
		}

	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			// process vertex
			Vertex vertex;
			glm::vec3 tempVec3;

			tempVec3.x = mesh->mVertices[i].x;
			tempVec3.y = mesh->mVertices[i].y;
			tempVec3.z = mesh->mVertices[i].z;
			vertex.Position = tempVec3;

			tempVec3.x = mesh->mNormals[i].x;
			tempVec3.y = mesh->mNormals[i].y;
			tempVec3.z = mesh->mNormals[i].z;
			vertex.Normal = tempVec3;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 tempVec2;

				tempVec2.x = mesh->mTextureCoords[0]->x;
				tempVec2.y = mesh->mTextureCoords[0]->y;
				vertex.TexCoord = tempVec2;
			}
			else
			{
				vertex.TexCoord = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// process index
		std::vector<uint32_t> indices;
		for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
			aiFace& face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// process texture
		std::vector<Shared<Texture2D>> textures;
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Shared<Texture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Shared<Texture2D>> specularMaps= LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			//Shared<Texture2D> texture = Texture2D::Create("");
			//textures.push_back(texture);
		}


		return Mesh(vertices, indices, textures);
	}

	std::vector<Shared<Texture2D>> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector<Shared<Texture2D>> textures;
		for (uint32_t i = 0; i < material->GetTextureCount(type); ++i)
		{
			aiString fileName;
			material->GetTexture(type, i, &fileName);
			Shared<Texture2D> texture = Texture2D::Create(m_Directory + fileName.C_Str());
			texture->SetTypeName(typeName);
			textures.push_back(texture);
		}


		return textures;
	}



}

