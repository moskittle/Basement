#include "bmpch.h"
#include "LayerStack.h"

namespace Basement {

	LayerStack::~LayerStack()
	{
		for (auto layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		++m_LayerInsertIndex;
		layer->Attach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto iter = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);

		if (iter != m_Layers.end())
		{
			layer->Detach();
			m_Layers.erase(iter);
			--m_LayerInsertIndex;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->Attach();
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto iter = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		
		if (iter != m_Layers.end())
		{
			overlay->Detach();
			m_Layers.erase(iter);
		}
	}

}