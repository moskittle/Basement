#include "cs560Layer.h"

#include "Basement/Renderer/Renderer.h"

cs560Layer::cs560Layer()
{
	Basement::Renderer::EnableDepthTest();

	BuildScene();
}

void cs560Layer::Update(const Basement::Timer& dt)
{
}

void cs560Layer::RenderImGui()
{
}

void cs560Layer::HandleEvent(Basement::Event& event)
{
}

void cs560Layer::BuildScene()
{
}

void cs560Layer::RenderScene()
{
}


