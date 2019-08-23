#include "bmpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Basement {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		BM_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		bool bIsInitialized = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BM_CORE_ASSERT(bIsInitialized, "Failed to initialize Glad!");

		BM_CORE_INFO("OpenGL Vendor:   {0}", glGetString(GL_VENDOR));
		BM_CORE_INFO("OpenGL Renderer: {0}", glGetString(GL_RENDERER));
		BM_CORE_INFO("OpenGL Verision: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}