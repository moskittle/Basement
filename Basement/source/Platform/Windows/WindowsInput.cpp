#include "bmpch.h"
#include "WindowsInput.h"
#include "Basement/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Basement {

#define GET_APP_NATIVE_WINDOW Application::GetInstance().GetWindow().GetNativeWindow()

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(GET_APP_NATIVE_WINDOW);
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		
		auto window = static_cast<GLFWwindow*>(GET_APP_NATIVE_WINDOW);
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(GET_APP_NATIVE_WINDOW);
		std::pair<double, double> mousePos;
		glfwGetCursorPos(window, &mousePos.first, &mousePos.second);

		return { static_cast<float>(mousePos.first), static_cast<float>(mousePos.second)};
	}

	float WindowsInput::GetMousePositionXImpl()
	{
		return GetMousePosition().first;
	}

	float WindowsInput::GetMousePositionYImpl()
	{
		return GetMousePosition().second;
	}

}