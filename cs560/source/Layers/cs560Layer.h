#pragma once

#include "Basement/Core/Layer.h"

#include "Basement/Renderer/CameraController.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Basement/Renderer/FrameBuffer.h"
#include "Basement/Renderer/VertexArray.h"
#include "Basement/Renderer/FrameBuffer.h"
#include "Basement/Renderer/Model.h"
#include "Basement/Renderer/Texture.h"



class cs560Layer : public Basement::Layer
{
public:
	cs560Layer();
	~cs560Layer() = default;


	virtual void Update(const Basement::Timer& dt) override;
	virtual void RenderImGui() override;
	virtual void HandleEvent(Basement::Event& event) override;
private:
	void BuildScene();
	void RenderScene();

	Basement::CameraController3D m_CameraController;
	Basement::ShaderLibrary m_ShaderLibrary;

	// Model
	//Basement::SharedPtr<Basement::Model> m_NanoSuit;
	Basement::SharedPtr<Basement::Model> m_YBot;

	// Floor
	Basement::SharedPtr<Basement::VertexArray> m_FloorVAO;
	Basement::SharedPtr<Basement::VertexBuffer> m_FloorVBO;
	Basement::SharedPtr<Basement::Texture2D> m_FloorTexture;

	// Screen Quad
	Basement::SharedPtr<Basement::VertexArray> m_ScreenVAO;
	Basement::SharedPtr<Basement::VertexBuffer> m_ScreenVBO;
	Basement::SharedPtr<Basement::Texture2D> m_ScreenTexture;


	// Skybox
	Basement::SharedPtr<Basement::VertexArray> m_SkyboxVAO;
	Basement::SharedPtr<Basement::VertexBuffer> m_SkyboxVBO;
	Basement::SharedPtr<Basement::TextureCube> m_SkyboxTexture;

	// FrameBuffer
	Basement::SharedPtr<Basement::FrameBuffer> m_FrameBuffer;
};
