#pragma once

#include "Basement/Core/Layer.h"

#include "Basement/Renderer/CameraController.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Basement/Renderer/Texture.h"
#include "Basement/Renderer/VertexArray.h"
#include "Basement/Renderer/Model.h"

#include <glm/glm.hpp>


class GoofyLandLayer : public Basement::Layer {
public:
	GoofyLandLayer();
	~GoofyLandLayer() = default;

	virtual void Update(const Basement::Timer& dt) override;
	virtual void RenderImGui() override;
	virtual void HandleEvent(Basement::Event& event) override;
private:
	void BuildScene();
	void RenderScene();

	void BuildLightingScene();
	void RenderLightingScene();

	void BuildLightingMapScene();
	void RenderLightingMapScene();

	void BuildModelScene();
	void RenderModelScene();

	void BuildSkyboxScene();
	void RednerSkyboxScene();
private:
	Basement::CameraController3D m_CameraController;
	
	Basement::ShaderLibrary m_ShaderLibrary;

	// Skybox
	Basement::SharedPtr<Basement::VertexArray> m_SkyboxVAO;
	Basement::SharedPtr<Basement::VertexBuffer> m_SkyboxVBO;
	Basement::SharedPtr<Basement::TextureCube> m_SkyboxTexture;

	// Object
	Basement::SharedPtr<Basement::VertexArray> m_VertexArray;
	Basement::SharedPtr<Basement::VertexBuffer> m_VertexBuffer;
	Basement::SharedPtr<Basement::IndexBuffer> m_IndexBuffer;
	Basement::SharedPtr<Basement::Texture2D> m_BoxTexture;
	Basement::SharedPtr<Basement::Texture2D> m_BoxSpecularTexture;
	Basement::SharedPtr<Basement::Texture2D> m_BoxEmissionTexture;

	// Model
	Basement::SharedPtr<Basement::Model> m_NanoSuit;

	// Light
	Basement::SharedPtr<Basement::VertexArray> m_LightVertexArray;
	Basement::SharedPtr<Basement::VertexBuffer> m_LightVertexBuffer;
	Basement::SharedPtr<Basement::IndexBuffer> m_LightIndexBuffer;

	// Floor
	Basement::SharedPtr<Basement::VertexArray> m_FloorVAO;
	Basement::SharedPtr<Basement::VertexBuffer> m_FloorVBO;
	Basement::SharedPtr<Basement::Texture2D> m_FloorTexture;

	// Sphere
	Basement::SharedPtr<Basement::VertexArray> m_SphereVAO;
	Basement::SharedPtr<Basement::VertexBuffer> m_SphereVBO;
	Basement::SharedPtr<Basement::IndexBuffer> m_SphereIBO;
};
