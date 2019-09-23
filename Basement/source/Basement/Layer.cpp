#include "bmpch.h"
#include "Layer.h"

namespace Basement {

	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{
	}

	void Layer::Attach()
	{
	}

	void Layer::Detach()
	{
	}

	void Layer::Update(const Timer& dt)
	{
	}

	void Layer::RenderImGui()
	{
	}

	void Layer::HandleEvent(Event& event)
	{
	}

}