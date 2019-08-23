#include <Basement.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Basement::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void Update() override
	{
		if (Basement::Input::IsKeyPressed(BM_KEY_TAB))
		{
			BM_TRACE("Tab key is pressed!(poll)");
		}
	}

	virtual void RenderImGui() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");

		ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;			// Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;			// Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;				// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;				// Enable Multi-Viewport / Platform Windows

		ImGui::End();
	}

	void ProcessEvent(Basement::Event& event) override
	{
		if (event.GetEventType() == Basement::EEventType::KeyPressed)
		{
			Basement::KeyPressedEvent& e = static_cast<Basement::KeyPressedEvent&>(event);
			BM_TRACE("{0}", (char)e.GetKeyCode());
			if (e.GetKeyCode() == BM_KEY_TAB)
			{
				BM_TRACE("Tab key is pressed!(event)");
			}
		}
	}
};

class Backyard : public Basement::Application
{
public:
	Backyard()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Basement::ImGuiLayer());
	}

	~Backyard() = default;

private:
	Backyard(const Backyard&) = delete;
	Backyard& operator=(const Backyard&) = delete;
};

Basement::Application* Basement::CreateApplication()
{
	return new Backyard();
}
