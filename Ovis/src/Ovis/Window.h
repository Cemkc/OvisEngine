#pragma once

#include "ovpch.h"

#include "Ovis/Core.h"
#include "Ovis/Events/Event.h"


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

	class OVIS_API Window
	{
	public:

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

	};
}