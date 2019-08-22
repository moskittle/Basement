#pragma once

#include "Basement/Input.h"

namespace Basement {

	class WindowsInput : public Input
	{
	public:
		WindowsInput() = default;
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMousePositionXImpl() override;
		virtual float GetMousePositionYImpl() override;
	private:
		WindowsInput(const WindowsInput&) = delete;
		WindowsInput& operator=(const WindowsInput&) = delete;
	};

}