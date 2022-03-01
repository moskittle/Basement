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
private:
	Basement::CameraController3D m_CameraController;

	Basement::ShaderLibrary m_ShaderLibrary;

	// Skybox
	Basement::SharedPtr<Basement::VertexArray> m_SkyboxVAO;
	Basement::SharedPtr<Basement::VertexBuffer> m_SkyboxVBO;
	Basement::SharedPtr<Basement::TextureCube> m_SkyboxTexture;

	// Monkey
	Basement::SharedPtr<Basement::Model> m_Monkey;
	Basement::SharedPtr<Basement::Model> m_Sphere;

	// Light
	Basement::SharedPtr<Basement::VertexArray> m_LightVertexArray;
	Basement::SharedPtr<Basement::VertexBuffer> m_LightVertexBuffer;
	Basement::SharedPtr<Basement::IndexBuffer> m_LightIndexBuffer;

	// Screen Quad
	Basement::SharedPtr<Basement::VertexArray> m_ScreenVAO;
	Basement::SharedPtr<Basement::VertexBuffer> m_ScreenVBO;
	Basement::SharedPtr<Basement::Texture2D> m_ScreenTexture;
	Basement::SharedPtr<Basement::VertexArray> m_SmallScreenVAO;
	Basement::SharedPtr<Basement::VertexBuffer> m_SmallScreenVBO;

	// Frame Buffer
	Basement::SharedPtr<Basement::FrameBuffer> m_FrameBuffer;

	// Uniform Buffer
	uint32_t m_CameraMatUBO;
};
