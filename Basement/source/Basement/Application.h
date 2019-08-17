#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Basement {
	
	class BASEMENT_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}