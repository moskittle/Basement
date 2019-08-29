#include "bmpch.h"
#include "Application.h"

#include "Basement/Log.h"
#include "Basement/Events/KeyEvent.h"
#include "Basement/Input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Basement {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BM_CORE_ASSERT(!s_Instance, "Application already exsists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BM_BIND_EVENT_FN(Application::ProcessEvent));

		m_ImGuiLayer = new ImGuiLayer();

		// Vertex Array
		m_VertexArray.reset(VertexArray::Create());

		// Vertex Buffer
		float vertices[4 * 7] = {
			// Position				// Color
			 0.5f, -0.5f, 0.0f,		0.75f, 0.25f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,		0.25f, 0.25f, 0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,		0.75f, 0.75f, 0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,		0.25f, 0.75f, 0.5f, 0.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(sizeof(vertices), vertices));

		BufferLayout bufferLayout = 
		{
			{ "a_Position", EShaderDataType::Float3 },
			{ "a_Color", EShaderDataType::Float4 }
		};
		m_VertexBuffer->SetLayout(bufferLayout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// Index Buffer
		uint32_t indices[3 * 2] = {
			0, 1, 2,
			1, 2, 3
		};
		m_IndexBuffer.reset(IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		// triangle
		float verticesTri[3 * 7] = {
			// Position				// Color
			 0.0f,  0.5f, 0.0f,		0.25f, 0.25f, 0.75f, 0.0f,
			-0.5f, -0.5f, 0.0f,		0.75f,  0.5f, 0.25f, 0.0f,
			 0.5f,  -0.5f, 0.0f,	0.25f, 0.25f, 0.5f, 0.0f,
		};

		uint32_t indicesTri[3 * 2] = {
			0, 1, 2,
			1, 2, 3
		};

		// Triangle
		m_TriangleVA.reset(VertexArray::Create());
		
		std::shared_ptr<VertexBuffer> triangleVB;
		triangleVB.reset(VertexBuffer::Create(sizeof(verticesTri), verticesTri));
		BufferLayout bufferLayoutTri =
		{
			{ "a_Position", EShaderDataType::Float3 },
			{ "a_Color", EShaderDataType::Float4 }
		};

		triangleVB->SetLayout(bufferLayoutTri);
		m_TriangleVA->AddVertexBuffer(triangleVB);
		
		std::shared_ptr<IndexBuffer> triangleIB;
		triangleIB.reset(IndexBuffer::Create(sizeof(indicesTri), indicesTri));
		m_TriangleVA->SetIndexBuffer(triangleIB);
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

			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_TriangleVA->Bind();
			glDrawElements(GL_TRIANGLES, m_TriangleVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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