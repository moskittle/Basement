#include "GoofyLandLayer.h"

#include "Basement/Renderer/RenderCommand.h"
#include "Basement/Renderer/Renderer.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

GoofyLandLayer::GoofyLandLayer() : Layer("GL"), m_CameraController(glm::vec3(0.0f, 0.0f, 3.0f), glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f)
{
	Basement::RenderCommand::EnableDepthTest();
	
	BuildScene();
}

void GoofyLandLayer::BuildScene()
{
	float cubeVertices[] = {
		// Position				// Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f
	};

	float vertices[] = {
		// positions			// texture coords
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	m_VertexArray = Basement::VertexArray::Create();

	m_VertexBuffer = Basement::VertexBuffer::Create(sizeof(cubeVertices), cubeVertices);

	Basement::BufferLayout bufferLayout= {
		{ Basement::EShaderDataType::Float3, "a_Position" },
		{ Basement::EShaderDataType::Float2, "a_TexCoord" }
	};
	m_VertexBuffer->SetLayout(bufferLayout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	Basement::Shared<Basement::Shader> textureShader = m_ShaderLibrary.Load("resource/shaders/Texture.glsl");
	
	m_BoxTexture = Basement::Texture2D::Create("resource/textures/mario-block.png");
	
	std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->UploadUniform1i("u_Texture", 0);
}

void GoofyLandLayer::Update(const Basement::Timer& dt)
{
	// Update
	m_CameraController.Update(dt);

	// Render
	Basement::RenderCommand::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	Basement::RenderCommand::Clear();

	Basement::Renderer::BeginScene(m_CameraController.GetCamera());

	auto textureShader = m_ShaderLibrary.Get("Texture");
	
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

	m_VertexArray->Bind();
	glm::mat4 model = glm::mat4(1.0f);

	for (int i = 0; i < 10; ++i)
	{
		model = glm::translate(glm::mat4(1.0f), cubePositions[i]) * 
				glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(10.0f + i * 20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		m_BoxTexture->Bind();
		Basement::Renderer::SubmitArrays(textureShader, m_VertexArray, 0, 36, model);
	}



	Basement::Renderer::EndScene();

}

void GoofyLandLayer::RenderImGui()
{
	ImGui::Begin("Hello World");

	ImGui::End();
}

void GoofyLandLayer::HandleEvent(Basement::Event& event)
{
}
