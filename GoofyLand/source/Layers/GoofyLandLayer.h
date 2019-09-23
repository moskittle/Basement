#pragma once

#include "Basement/Layer.h"
#include "Basement/Core/CameraController.h"
#include "Platform/OpenGL/OpenGLShader.h"

class GoofyLandLayer : public Basement::Layer {
public:
	GoofyLandLayer();
	~GoofyLandLayer() = default;

	void BuildScene();

	virtual void Update(const Basement::Timer& dt) override;
	virtual void RenderImGui() override;
	virtual void HandleEvent(Basement::Event& event) override;
private:
	Basement::OrthographicCameraController m_CameraController;
	Basement::ShaderLibrary m_ShaderLibrary;
};
