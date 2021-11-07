#include "bmpch.h"

#include "Model.h"

#include <unordered_map>
#include <glm/glm.hpp>

namespace Basement {

	static const u32 s_MeshImportFlags =
		aiProcess_Triangulate |
		aiProcess_FlipUVs;

	static std::unordered_map<std::string, SharedPtr<Texture2D>> TextureLibrary;

	SharedPtr<Model> Model::Create(const std::string& filePath)
	{
		return std::make_shared<Model>(filePath);
	}

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
			BM_CORE_ERROR("{0}", m_Importer->GetErrorString());
			return;
		}

		m_Directory = filePath.substr(0, filePath.find_last_of('/'));

		ProcessNode(m_Scene->mRootNode, m_Scene);
	}

	void Model::Draw(SharedPtr<Shader> shader, const glm::mat4& model)
	{
		for (auto& mesh : m_Meshes)
		{
			mesh.Draw(shader, model);
		}
	}

	void Model::DrawOutline(SharedPtr<Shader> shader, const glm::mat4& model)
	{
		for (auto& mesh : m_Meshes)
		{
			mesh.DrawOutline(shader, model);
		}
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (u32 i = 0; i < node->mNumMeshes; ++i)
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
		// process vertices
		std::vector<Vertex> vertices;
		ProcessVertices(mesh, m_Scene, vertices);

		// process index
		std::vector<u32> indices;
		ProcessIndices(mesh, m_Scene, indices);

		// process material
		MaterialAttrib materialAttrib;
		std::vector<SharedPtr<Texture2D>> textures;
		if (mesh->mMaterialIndex >= 0)
		{
			ProcessMaterial(mesh, m_Scene, materialAttrib, textures);
		}

		// process embedded texture
		//auto embeddedTex = GetEmbeddedTexture(const char* filename)

		return Mesh(vertices, indices, textures, materialAttrib);
	}

	void Model::ProcessVertices(aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices)
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			// process vertex
			Vertex vertex;
			vertex.Position = { mesh->mVertices[i].x , mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x , mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (mesh->HasTextureCoords(0))
			{
				vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			}
			else
			{
				vertex.TexCoord = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}
	}

	void Model::ProcessIndices(aiMesh* mesh, const aiScene* scene, std::vector<u32>& indices)
	{
		for (u32 i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace& face = mesh->mFaces[i];
			for (u32 j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
	}

	void Model::ProcessMaterial(aiMesh* mesh, const aiScene* scene, MaterialAttrib& materialAttrib, std::vector<SharedPtr<Texture2D>>& textures)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//// texture
		//std::vector<SharedPtr<Texture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		//std::vector<SharedPtr<Texture2D>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		//std::vector<Shared<Texture2D>> normalMaps= LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//
		//std::vector<Shared<Texture2D>> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// material attributes
		aiColor4D ambientColor, diffuseColor, specularColor;
		float shininess;

		aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specularColor);
		aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

		glm::vec3 ambient(ambientColor.r, ambientColor.g, ambientColor.b);
		glm::vec3 diffuse(diffuseColor.r, diffuseColor.g, diffuseColor.b);
		glm::vec3 specular(specularColor.r, specularColor.g, specularColor.b);

		materialAttrib.SetAttributes(ambientColor, diffuseColor, specularColor, shininess);
	}

	std::vector<SharedPtr<Texture2D>> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector<SharedPtr<Texture2D>> textures;
		for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			std::string fileName(str.C_Str());

			BM_CORE_INFO("Load Texture: {0}", str.C_Str());

			if (TextureLibrary.find(fileName) != TextureLibrary.end())
			{
				BM_CORE_WARN("Texture {0} already exists!", fileName);
			}
			else
			{
				SharedPtr<Texture2D> texture = Texture2D::Create(m_Directory + "/" + fileName);
				texture->SetTypeName(typeName);
				textures.push_back(texture);

				TextureLibrary[fileName] = texture;
			}
		}

		return textures;
	}

}

