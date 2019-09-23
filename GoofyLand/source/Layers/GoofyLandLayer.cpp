#include "GoofyLandLayer.h"

#include "Basement/Renderer/RenderCommand.h"
#include "Basement/Renderer/VertexArray.h"
#include "Basement/Renderer/Texture.h"

#include "ImGui/imgui.h"

//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

GoofyLandLayer::GoofyLandLayer() : Layer("GL"), m_CameraController(1280.0f/720.0f)
{
	Basement::RenderCommand::EnableDepthTest();
	
	BuildScene();
}

void GoofyLandLayer::BuildScene()
{
	float boxVertices[] = {
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

	Basement::Shared<Basement::VertexArray> vertexArray = Basement::VertexArray::Create();

	Basement::Shared<Basement::VertexBuffer> vertexBuffer = Basement::VertexBuffer::Create(sizeof(vertices), vertices);

	// buffer layout
	Basement::BufferLayout bufferLayout= {
		{ Basement::EShaderDataType::Float3, "a_Position" },
		{ Basement::EShaderDataType::Float2, "a_TexCoord" }
	};
	vertexBuffer->SetLayout(bufferLayout);
	vertexArray->AddVertexBuffer(vertexBuffer);

	// index buffer
	Basement::Shared<Basement::IndexBuffer> indexBuffer = Basement::IndexBuffer::Create(sizeof(indices), indices);
	vertexArray->SetIndexBuffer(indexBuffer);

	// shaders
	Basement::Shared<Basement::Shader> textureShader = m_ShaderLibrary.Load("resource/shaders/Texture.glsl");
	
	// texture
	Basement::Shared<Basement::Texture2D> boxTexture = Basement::Texture2D::Create("resource/textures/container.jpg");
	
	std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->UploadUniform1i("u_Texture", 0);

}

void GoofyLandLayer::Update(const Basement::Timer& dt)
{
	// Update
	m_CameraController.Update(dt);

	// Render
	Basement::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.6f, 0.8f, 1.0f));
	Basement::RenderCommand::Clear();



}

void GoofyLandLayer::RenderImGui()
{
	ImGui::Begin("Hello World");

	ImGui::End();
}

void GoofyLandLayer::HandleEvent(Basement::Event& event)
{
}
