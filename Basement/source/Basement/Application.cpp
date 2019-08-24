#include "bmpch.h"
#include "Application.h"

#include "Basement/Log.h"
#include "Basement/Events/KeyEvent.h"
#include "Basement/Input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Basement {

	Application* Application::s_Instance = nullptr;

	// TODO move to class as Getter()
	static GLenum ShaderDataTypeToOpenGLBaseType(EShaderDataType type) 
	{
		switch (type)
		{
			case EShaderDataType::Int:      return GL_INT;
			case EShaderDataType::Int2:     return GL_INT;
			case EShaderDataType::Int3:     return GL_INT;
			case EShaderDataType::Int4:     return GL_INT;
			case EShaderDataType::Float:    return GL_FLOAT;
			case EShaderDataType::Float2:   return GL_FLOAT;
			case EShaderDataType::Float3:   return GL_FLOAT;
			case EShaderDataType::Float4:   return GL_FLOAT;
			case EShaderDataType::Mat3:     return GL_FLOAT;
			case EShaderDataType::Mat4:     return GL_FLOAT;
			case EShaderDataType::Bool:     return GL_BOOL;
		}

		BM_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	Application::Application()
	{
		BM_CORE_ASSERT(!s_Instance, "Application already exsists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BM_BIND_EVENT_FN(Application::ProcessEvent));

		m_ImGuiLayer = new ImGuiLayer();

		// Vertex Array
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// Vertex Buffer
		float vertices[4 * 7] = {
			// Position				// Color
			 0.5f, -0.5f, 0.0f,		0.75f, 0.25f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,		0.25f, 0.25f, 0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,		0.75f, 0.75f, 0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,		0.25f, 0.75f, 0.5f, 0.0f
			//// Position				// Color
			// 1.0f, -1.0f, 0.0f,		0.75f, 0.25f, 0.5f, 0.0f,
			//-1.0f, -1.0f, 0.0f,		0.25f, 0.25f, 0.5f, 0.0f,
			// 1.0f,  1.0f, 0.0f,		0.75f, 0.75f, 0.5f, 0.0f,
			//-1.0f,  1.0f, 0.0f,		0.25f, 0.75f, 0.5f, 0.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(sizeof(vertices), vertices));

		BufferLayout bufferLayout = 
		{
			{ "a_Position", EShaderDataType::Float3 },
			{ "a_Color", EShaderDataType::Float4 }
		};
		m_VertexBuffer->SetLayout(bufferLayout);

		uint32_t index = 0;
		for (const auto& element : m_VertexBuffer->GetLayout())
		{
			// Set vertex attributes
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.bIsNormalized ? GL_TRUE : GL_FALSE, 
				m_VertexBuffer->GetLayout().GetStride(),
				(const void*)(intptr_t)element.Offset);

			++index;
		}
		

		// temp
		{
			//glEnableVertexAttribArray(1);
			//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3));
		}

		// Index Buffer
		uint32_t indices[3 * 2] = { 
			0, 1, 2,
			1, 2, 3
		};
		m_IndexBuffer.reset(IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));


		// Shaders
		const std::string vertSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Color;
			out vec4 v_Color2;

			void main()
			{
				//v_Color = a_Position;
				v_Color2 = a_Color;
				gl_Position = vec4(a_Position, 1.0);
				//gl_Position = vec4(a_Position - 0.5, 1.0);
			}
		)";

		const std::string fragSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Color;
			in vec4 v_Color2;
			
			void main()
			{
				//color = vec4(v_Color * 0.5f + 0.5f, 0.0);
				color = v_Color2;
			}
		)";

		m_Shader.reset(new Shader(vertSource, fragSource));
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			float div = 256.0f;
			float green[3] = { 105.0f / div, 190.0f / div, 40.0f / div };	// action green	
			float blue[3] = { 0.0f, 34.0f / div, 68.0f / div };				// college navy
			float grey[3] = { 165.0f / div, 172.0f / div, 175.0f / div };	// wolf grey

			// Background color
			glClearColor( grey[0], grey[1], grey[2], 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();

			//m_IndexBuffer->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (auto& layer : m_LayerStack)
			{
				layer->Update();
			}

			m_ImGuiLayer->Begin();
			for (auto& layer : m_LayerStack)
			{
				layer->RenderImGui();
			}
			m_ImGuiLayer->End();
			
			m_Window->Update();
		}
	}

	void Application::ProcessEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BM_BIND_EVENT_FN(Application::CloseWindow));

		// BM_CORE_TRACE("{0}", event);
		
		for (auto iter = m_LayerStack.end(); iter != m_LayerStack.begin();)
		{
			(*--iter)->ProcessEvent(event);

			if (event.m_Handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	bool Application::CloseWindow(WindowCloseEvent& event)
	{
		m_IsRunning = false;

		return true;
	}

}