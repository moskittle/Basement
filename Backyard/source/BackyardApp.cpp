#include "Basement.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Basement::Layer
{
	const float div = 256.0f;
	const glm::vec3 green = { 105.0f / div, 190.0f / div, 40.0f / div };	// action green	
	const glm::vec3 navy = { 0.0f, 34.0f / div, 68.0f / div };			// college navy
	const glm::vec3 grey = { 165.0f / div, 172.0f / div, 175.0f / div };	// wolf grey
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), /*m_Camera(m_Position),*/ m_CameraPosition(0.0f), m_CameraSpeed(5.0f),
		m_CameraRotation(0.0f), m_CameraRotationSpeed(90.0f),
		m_Position(0.0f), m_MoveSpeed(1.0f), m_Scale(glm::vec3(0.1f)), m_ModelMatrix(glm::mat4(1.0f)),
		m_SquareColor(navy)
	{
		// Vertex Array
		m_VertexArray.reset(Basement::VertexArray::Create());

		// Vertex Buffer
		float vertices[4 * 9] = {
			// Position				// Color						// Texture Coord
			-0.5f, -0.5f, 0.0f,		0.75f, 0.25f, 0.5f, 0.0f,		0.0f, 0.0f,			// bottom-left
			-0.5f,  0.5f, 0.0f,		0.25f, 0.25f, 0.5f, 0.0f,		0.0f, 1.0f,			// top-left
			 0.5f,  0.5f, 0.0f,		0.75f, 0.75f, 0.5f, 0.0f,		1.0f, 1.0f,			// top-right
			 0.5f, -0.5f, 0.0f,		0.25f, 0.75f, 0.5f, 0.0f,		1.0f, 0.0f			// bottom-right
		};
		m_VertexBuffer.reset(Basement::VertexBuffer::Create(sizeof(vertices), vertices));

		Basement::BufferLayout bufferLayout =
		{
			{ Basement::EShaderDataType::Float3, "a_Position" },
			{ Basement::EShaderDataType::Float4, "a_Color" },
			{ Basement::EShaderDataType::Float2, "a_TextureCoord" }
		};
		m_VertexBuffer->SetLayout(bufferLayout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// Index Buffer
		uint32_t indices[3 * 2] = {
			0, 1, 2,
			2, 3, 0	
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

		Basement::Shared<Basement::Shader> flatColorShader = m_ShaderLibrary.Load("resource/shaders/FlatColor.glsl");
		Basement::Shared<Basement::Shader> textureShader = m_ShaderLibrary.Load("resource/shaders/Texture.glsl");


		//m_Texture.reset(Basement::Texture2D::Create("resource/textures/bwag_art.jpg"));
		//m_Texture.reset(Basement::Texture2D::Create("resource/textures/awesomeface.png"));
		//m_Texture2.reset(Basement::Texture2D::Create("resource/textures/seahawks_logo.png"));
		m_Texture.reset(Basement::Texture2D::Create("resource/textures/bwag.jpg"));
		m_Texture2.reset(Basement::Texture2D::Create("resource/textures/seahawks_logo.tga"));

		std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->UploadUniform1i("u_Texture", 0);	// slot: 0
	}

	void Update(const Basement::Timer& deltaTime) override
	{
		BM_TRACE("Delta Time: {0}s ({1}fps)", deltaTime.GetDeltaTimeInSeconds(), deltaTime.GetFramePerSecond());
		
		//BM_TRACE("Camera Position: {0}, {1}, {2})", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
		//static int frame = 0;
		//BM_CORE_TRACE("Frame({0})", ++frame);

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
		//if (Basement::Input::IsKeyPressed(BM_KEY_R))
		//{
		//	m_CameraPosition.z -= m_CameraSpeed * deltaTime;
		//}
		//else if (Basement::Input::IsKeyPressed(BM_KEY_F))
		//{
		//	m_CameraPosition.z += m_CameraSpeed * deltaTime;
		//}

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

		Basement::RenderCommand::SetClearColor(glm::vec4(grey, 1.0f));
		Basement::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Basement::Renderer::BeginScene(m_Camera);

		auto flatColorShader = m_ShaderLibrary.Get("FlatColor");
		auto textureShader = m_ShaderLibrary.Get("Texture");

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);
		for (int y = 0; y < 10; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				m_ModelMatrix = glm::translate(glm::mat4(1.0f), position) * scale;

				std::dynamic_pointer_cast<Basement::OpenGLShader>(flatColorShader)->Bind();
				std::dynamic_pointer_cast<Basement::OpenGLShader>(flatColorShader)->UploadUniform3f("u_Color", m_SquareColor);

				Basement::Renderer::Submit(flatColorShader, m_VertexArray, m_ModelMatrix);
			}
		}

		m_Texture->Bind();
		Basement::Renderer::Submit(textureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_Texture2->Bind();
		Basement::Renderer::Submit(textureShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		//m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) * scale;
		//Basement::Renderer::Submit(m_FlatColorShader, m_TriangleVA, m_ModelMatrix);

		Basement::Renderer::EndScene();
	}

	virtual void RenderImGui() override
	{
		ImGui::Begin("Color Picker");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		
		ImGui::End();
	}

	void HandleEvent(Basement::Event& event) override
	{
	}

private:
	Basement::ShaderLibrary m_ShaderLibrary;

	Basement::Shared<Basement::VertexArray> m_VertexArray;
	Basement::Shared<Basement::VertexBuffer> m_VertexBuffer;
	Basement::Shared<Basement::IndexBuffer> m_IndexBuffer;

	Basement::Shared<Basement::Texture2D> m_Texture, m_Texture2;

	Basement::Shared<Basement::VertexArray> m_TriangleVA;

	glm::vec3 m_CameraPosition;
	Basement::OrthographicCamera m_Camera;
	float m_CameraSpeed;
	float m_CameraRotation;
	float m_CameraRotationSpeed;

	glm::mat4 m_ModelMatrix;
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	float m_MoveSpeed;

	glm::vec3 m_SquareColor;
};

class Backyard : public Basement::Application
{
public:
	Backyard()
	{
		PushLayer(new ExampleLayer());
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
