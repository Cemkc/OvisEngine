#pragma once

#include "Ovis/Core.h"
#include "Ovis/Events/Event.h"
#include "Ovis/Renderer/GraphicsContext.h"

namespace Ovis {


	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string title = "Ovis Engine", 
					unsigned int widht = 1280,
					unsigned int height = 720)
					:Title(title), Width(widht), Height(height){}
	};

	class Window
	{
	protected:
		GraphicsContext* m_Context;
	public:

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual double GetTime() const = 0;

		// To be defined by the window class of a particular platform
		static Window* Create(const WindowProps& props = WindowProps());

	};
}