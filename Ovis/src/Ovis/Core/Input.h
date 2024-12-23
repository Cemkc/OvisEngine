#pragma once

#include "Core.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Ovis 
{
	class Input 
	{
	public:
		static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		static bool IsMouseButtonPressed(int button) { return s_Instance->IsKeyPressedImpl(button); }
		static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		static float GetMouseX() { return s_Instance->GetMouseX(); }
		static float GetMouseY() { return s_Instance->GetMouseY(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* s_Instance;
	};
}