#pragma once

#include "Basement/Layer.h"

#include "Basement/Core/CameraController.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Basement/Renderer/Texture.h"
#include "Basement/Renderer/VertexArray.h"


class GoofyLandLayer : public Basement::Layer {
public:
	GoofyLandLayer();
	~GoofyLandLayer() = default;

	void BuildScene();
	void RenderScene();

	void BuildLightingScene();
	void RenderLightingScene();

	virtual void Update(const Basement::Timer& dt) override;
	virtual void RenderImGui() override;
	virtual void HandleEvent(Basement::Event& event) override;
private:
	Basement::CameraController3D m_CameraController;
	
	Basement::ShaderLibrary m_ShaderLibrary;
	Basement::Shared<Basement::VertexArray> m_VertexArray;
	Basement::Shared<Basement::VertexBuffer> m_VertexBuffer;
	Basement::Shared<Basement::IndexBuffer> m_IndexBuffer;

	Basement::Shared<Basement::VertexArray> m_LightVertexArray;
	Basement::Shared<Basement::VertexBuffer> m_LightVertexBuffer;
	Basement::Shared<Basement::IndexBuffer> m_LightIndexBuffer;

	Basement::Shared<Basement::Texture2D> m_BoxTexture;

	// Sphere
	Basement::Shared<Basement::VertexArray> m_SphereVAO;
	Basement::Shared<Basement::VertexBuffer> m_SphereVBO;
	Basement::Shared<Basement::IndexBuffer> m_SphereIBO;
};
