#include "Basement.h"
#include "Layers/cs560Layer.h"

class cs560App : public Basement::Application
{
public:
	cs560App()
	{
		BM_INFO("Welcome to CS560 Project");

		PushLayer(new cs560Layer());
	}

	~cs560App() = default;
};


Basement::Application* Basement::CreateApplication() {
	return new cs560App();
}