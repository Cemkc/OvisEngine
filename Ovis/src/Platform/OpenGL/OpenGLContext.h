#pragma once

#include "Ovis/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Ovis 
{
	class OpenGLContext : public GraphicsContext
	{
	private:
		GLFWwindow* m_Window;
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	};
}