#include "ovpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Ovis 
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_Window(windowHandle)
	{
		OV_CORE_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OV_CORE_ASSERT(status, "Failed to initialize Glad!");

		OV_CORE_INFO("OpenGL Info:");
		OV_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		OV_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		OV_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}