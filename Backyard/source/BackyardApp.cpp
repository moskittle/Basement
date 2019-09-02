#include "Basement.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Basement::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraSpeed(5.0f), 
		m_CameraRotation(0.0f), m_CameraRotationSpeed(90.0f), m_Position(0.0f), m_MoveSpeed(1.0f), m_Scale(glm::vec3(0.1f))
	{
		// Vertex Array
		m_VertexArray.reset(Basement::VertexArray::Create());

		// Vertex Buffer
		float vertices[4 * 7] = {
			// Position				// Color
			 0.5f, -0.5f, 0.0f,		0.75f, 0.25f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,		0.25f, 0.25f, 0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,		0.75f, 0.75f, 0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,		0.25f, 0.75f, 0.5f, 0.0f
		};
		m_VertexBuffer.reset(Basement::VertexBuffer::Create(sizeof(vertices), vertices));

		Basement::BufferLayout bufferLayout =
		{
			{ "a_Position", Basement::EShaderDataType::Float3 },
			{ "a_Color", Basement::EShaderDataType::Float4 }
		};
		m_VertexBuffer->SetLayout(bufferLayout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// Index Buffer
		uint32_t indices[3 * 2] = {
			0, 1, 2,
			1, 2, 3
		};
		m_IndexBuffer.reset(Basement::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// triangle
		float verticesTri[3 * 7] = {
			// Position				// Color
			 0.0f,  0.5f, 0.0f,		0.25f, 0.25f, 0.75f, 0.0f,
			-0.5f, -0.5f, 0.0f,		0.75f,  0.5f, 0.25f, 0.0f,
			 0.5f,  -0.5f, 0.0f,	0.25f, 0.25f, 0.5f, 0.0f,
		};

		uint32_t indicesTri[3] = {
			0, 1, 2,
		};

		// Triangle
		m_TriangleVA.reset(Basement::VertexArray::Create());

		std::shared_ptr<Basement::VertexBuffer> VBTriangle;
		VBTriangle.reset(Basement::VertexBuffer::Create(sizeof(verticesTri), verticesTri));
		Basement::BufferLayout bufferLayoutTri =
		{
			{ "a_Position", Basement::EShaderDataType::Float3 },
			{ "a_Color", Basement::EShaderDataType::Float4 }
		};

		VBTriangle->SetLayout(bufferLayoutTri);
		m_TriangleVA->AddVertexBuffer(VBTriangle);

		std::shared_ptr<Basement::IndexBuffer> IBTriangle;
		IBTriangle.reset(Basement::IndexBuffer::Create(sizeof(indicesTri), indicesTri));
		m_TriangleVA->SetIndexBuffer(IBTriangle);

		// Shaders
		const std::string vertSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_ModelMatrix;

			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position, 1.0);
			}
		)";

		const std::string fragSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
		)";

		m_ShaderProgram.reset(new Basement::ShaderProgram(vertSource, fragSource));
	}

	void Update(const Basement::Timer& deltaTime) override
	{
		//BM_TRACE("Delta Time: {0}s ({1}ms)", deltaTime.GetDeltaTimeInSeconds(), deltaTime.GetDeltaTimeInMilliseconds());
		BM_TRACE("Position: ({0}, {1}, {2})", m_Position.x, m_Position.y, m_Position.z);

		// Camera Movement
		if (Basement::Input::IsKeyPressed(BM_KEY_W))
		{
			m_CameraPosition.y += m_CameraSpeed * deltaTime;
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_S))
		{
			m_CameraPosition.y -= m_CameraSpeed * deltaTime;
		}
		if (Basement::Input::IsKeyPressed(BM_KEY_A))
		{
			m_CameraPosition.x -= m_CameraSpeed * deltaTime;
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_D))
		{
			m_CameraPosition.x += m_CameraSpeed * deltaTime;
		}

		// Camera Rotation
		if (Basement::Input::IsKeyPressed(BM_KEY_Q))
		{
			m_CameraRotation += m_CameraRotationSpeed * deltaTime;
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_E))
		{
			m_CameraRotation -= m_CameraRotationSpeed * deltaTime;
		}

		// Transform Movement
		if (Basement::Input::IsKeyPressed(BM_KEY_I))
		{
			m_Position.y += m_MoveSpeed * deltaTime;
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_K))
		{
			m_Position.y -= m_MoveSpeed * deltaTime;
		}
		if (Basement::Input::IsKeyPressed(BM_KEY_J))
		{
			m_Position.x -= m_MoveSpeed * deltaTime;
		}
		else if (Basement::Input::IsKeyPressed(BM_KEY_L))
		{
			m_Position.x += m_MoveSpeed * deltaTime;
		}


		static float div = 256.0f;
		static glm::vec4 green = { 105.0f / div, 190.0f / div, 40.0f / div, 0.0f };	// action green	
		static glm::vec4 navy = { 0.0f, 34.0f / div, 68.0f / div, 0.0f };			// college navy
		static glm::vec4 grey = { 165.0f / div, 172.0f / div, 175.0f / div, 0.0f };	// wolf grey


		Basement::RenderCommand::SetClearColor(navy);
		Basement::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Basement::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);

		for (int y = 0; y < 10; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), position) * scale;
				Basement::Renderer::Submit(m_ShaderProgram, m_VertexArray, ModelMatrix);
			}
		}

		//Basement::Renderer::Submit(m_ShaderProgram, m_TriangleVA, ModelMatrix);

		Basement::Renderer::EndScene();
	}

	virtual void RenderImGui() override
	{
		
	}

	void ProcessEvent(Basement::Event& event) override
	{
	}

private:
	std::shared_ptr<Basement::ShaderProgram> m_ShaderProgram;

	std::shared_ptr<Basement::VertexArray> m_VertexArray;
	std::shared_ptr<Basement::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Basement::IndexBuffer> m_IndexBuffer;

	std::shared_ptr<Basement::VertexArray> m_TriangleVA;

	Basement::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed;
	float m_CameraRotation;
	float m_CameraRotationSpeed;

	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	float m_MoveSpeed;
};

class Backyard : public Basement::Application
{
public:
	Backyard()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Basement::ImGuiLayer());
	}

	~Backyard() = default;
private:
	Backyard(const Backyard&) = delete;
	Backyard& operator=(const Backyard&) = delete;
};

Basement::Application* Basement::CreateApplication()
{
	return new Backyard();
}
