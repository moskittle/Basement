#include <Basement.h>

class ExampleLayer : public Basement::Layer
{
public:
	ExampleLayer()
		: Layer("Example") 
	{
	}

	void Update() override
	{
		BM_INFO("ExampleLayer::Update");
	}

	void ProcessEvent(Basement::Event& event) override
	{
		BM_TRACE("{0}", event);
	}
};

class Backyard : public Basement::Application
{
public:
	Backyard()
	{
		//PushLayer(new ExampleLayer());
		PushOverlay(new Basement::ImGuiLayer());
	}

	~Backyard()
	{
	}
};

Basement::Application* Basement::CreateApplication()
{
	return new Backyard();
}
