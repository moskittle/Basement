#include "Basement.h"

#include "layers/GoofyLandLayer.h"


class GoofyLandApp : public Basement::Application
{
public:
	GoofyLandApp()
	{
		BM_WARN("Entering Goofy Land...");

		PushLayer(new GoofyLandLayer());
	}
	~GoofyLandApp() = default;
};

Basement::Application* Basement::CreateApplication() {
	return new GoofyLandApp();
}