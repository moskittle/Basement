#pragma once

#include "Core.h"

namespace Basement {
	
	class BASEMENT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};


}