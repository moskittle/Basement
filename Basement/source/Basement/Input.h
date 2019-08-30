#pragma once

#include "Basement/Core.h"

namespace Basement {

	class Input
	{
	protected:
		Input() = default;
	public:
		virtual ~Input() = default;

		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMousePositionX() { return s_Instance->GetMousePositionXImpl(); }
		inline static float GetMousePositionY() { return s_Instance->GetMousePositionYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMousePositionXImpl() = 0;
		virtual float GetMousePositionYImpl() = 0;
	private:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
	private:
		static Input* s_Instance;
	};

}