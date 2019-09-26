#include "GoofyLandLayer.h"

#include "Basement/Renderer/RenderCommand.h"
#include "Basement/Renderer/Renderer.h"
#include "Basement/Mesh/Sphere.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

// Global Variables
glm::vec3 LightPosition(1.2f, 1.0f, 2.0f);

glm::vec3 White(1.0f);
glm::vec3 Blue(0.5f, 0.9f, 0.9f);



GoofyLandLayer::GoofyLandLayer() : Layer("GL"), m_CameraController(glm::vec3(0.0f, 0.0f, 5.0f), 45.0f, 1.7778f, 0.1f, 100.0f)
{
	Basement::RenderCommand::EnableDepthTest();
	
	//BuildScene();
	BuildLightingScene();
}

void GoofyLandLayer::BuildScene()
{
	float cubeVertices[] = {
	// Position				// Texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	// Cubes
	// VAO
	m_VertexArray = Basement::VertexArray::Create();

	// VBO
	m_VertexBuffer = Basement::VertexBuffer::Create(sizeof(cubeVertices), cubeVertices);
	Basement::BufferLayout bufferLayout = {
		{ Basement::EShaderDataType::Float3, "a_Position" },
		{ Basement::EShaderDataType::Float2, "a_TexCoord" }
	};
	m_VertexBuffer->SetLayout(bufferLayout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	// Shader
	Basement::Shared<Basement::Shader> textureShader = m_ShaderLibrary.Load("resource/shaders/Texture.glsl");
	std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->UploadUniform1i("u_Texture", 0);

	// Texture
	m_BoxTexture = Basement::Texture2D::Create("resource/textures/mario-block.png");


	//// Light

	////// VAO
	//m_LightVertexArray = Basement::VertexArray::Create();

	////VBO
	//m_LightVertexBuffer = Basement::VertexBuffer::Create(sizeof(cubeVertices), cubeVertices);
	////Basement::BufferLayout bufferLayout = {
	////	{ Basement::EShaderDataType::Float3, "a_Position" },
	////	{ Basement::EShaderDataType::Float2, "a_TexCoord"}
	////};
	//m_LightVertexBuffer->SetLayout(bufferLayout);
	//m_VertexArray->AddVertexBuffer(m_LightVertexBuffer);

	//// Shader
	////Basement::Shared<Basement::Shader> 

	

	//// Sphere
	//{
	//	Basement::Shared<Basement::Sphere> sphere = std::make_shared<Basement::Sphere>(30);

	//	// VAO
	//	m_SphereVAO = Basement::VertexArray::Create();
	//	// VBO
	//	m_SphereVBO = Basement::VertexBuffer::Create(4 * sphere->GetVertexCount(), sphere->GetVertexArray());
	//	Basement::BufferLayout sphereBufferLayout = {
	//		{Basement::EShaderDataType::Float4, "a_Position"}
	//	};
	//	m_SphereVBO->SetLayout(sphereBufferLayout);
	//	m_SphereVAO->AddVertexBuffer(m_VertexBuffer);

	//	// IBO
	//	m_SphereIBO = Basement::IndexBuffer::Create(3 * sphere->GetFaceCount(), sphere->GetFaceArray());
	//	m_SphereVAO->SetIndexBuffer(m_SphereIBO);

	//	// Shader
	//	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	//	Basement::Shared<Basement::Shader> flatColorShader = m_ShaderLibrary.Load("resource/shaders/FlatColor.glsl");
	//	//std::dynamic_pointer_cast<Basement::OpenGLShader>(flatColorShader)->UploadUniform4f("u_Color", color);
	//}
}

void GoofyLandLayer::RenderScene()
{
	auto textureShader = m_ShaderLibrary.Get("Texture");

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	m_VertexArray->Bind();

	glm::mat4 model = glm::mat4(1.0f);
	//for (int i = 0; i < 10; ++i)
	//{
	//	model = glm::translate(glm::mat4(1.0f), cubePositions[i]) * 
	//			glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(10.0f + i * 20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
	//	m_BoxTexture->Bind();
	//	Basement::Renderer::SubmitArrays(textureShader, m_VertexArray, 0, 36, model);
	//}

	// Box
	model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_BoxTexture->Bind();
	Basement::Renderer::SubmitArrays(textureShader, m_VertexArray, 0, 36, model);

	//// Sphere
	//auto flatColorShader = m_ShaderLibrary.Get("FlatColor");
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//Basement::Renderer::Submit(flatColorShader, m_SphereVAO, model);
}


void GoofyLandLayer::BuildLightingScene()
{
	float boxVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	float lightVertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
	};

	//------------------------
	// Box
	//------------------------

	// VAO
	m_VertexArray = Basement::VertexArray::Create();

	// VBO
	m_VertexBuffer = Basement::VertexBuffer::Create(sizeof(boxVertices), boxVertices);
	Basement::BufferLayout bufferLayout = {
		{ Basement::EShaderDataType::Float3, "a_Position" }, 
		{ Basement::EShaderDataType::Float2, "a_TexCoord" }
	};
	m_VertexBuffer->SetLayout(bufferLayout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	// Shader
	Basement::Shared<Basement::Shader> lightingShader = m_ShaderLibrary.Load("resource/shaders/Lighting.glsl");
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingShader)->UploadUniform3f("u_ObjectColor", Blue);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingShader)->UploadUniform3f("u_LightColor", White);
	
	//-----------------------
	// Light
	//-----------------------

	// VAO
	m_LightVertexArray = Basement::VertexArray::Create();
	
	// VBO
	m_LightVertexBuffer = Basement::VertexBuffer::Create(sizeof(lightVertices), lightVertices);
	Basement::BufferLayout lightingLayout = {
		{ Basement::EShaderDataType::Float3, "a_Position"}
	};
	m_LightVertexBuffer->SetLayout(lightingLayout);
	m_LightVertexArray->AddVertexBuffer(m_LightVertexBuffer);

	Basement::Shared<Basement::Shader> lightSourceShader = m_ShaderLibrary.Load("resource/shaders/LightSource.glsl");

}

void GoofyLandLayer::RenderLightingScene()
{
	auto lightingShader = m_ShaderLibrary.Get("Lighting");
	auto lightSourceShader = m_ShaderLibrary.Get("LightSource");

	glm::mat4 lightSourceModel = glm::translate(glm::mat4(1.0f), LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	Basement::Renderer::SubmitArrays(lightSourceShader, m_LightVertexArray, 0, 36, lightSourceModel);
	
	glm::mat4 objectModel = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	Basement::Renderer::SubmitArrays(lightingShader, m_VertexArray, 0, 36, objectModel);


	//// Light
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f));
	//Basement::Renderer::SubmitArrays(textureShader, m_LightVertexArray, 0, 36, model);
}



void GoofyLandLayer::Update(const Basement::Timer& dt)
{
	//BM_INFO("FPS: {0}", dt.GetFramePerSecond());

	// Update
	m_CameraController.Update(dt);

	// Render
	Basement::RenderCommand::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	Basement::RenderCommand::Clear();

	Basement::Renderer::BeginScene(m_CameraController.GetCamera());

	//RenderScene();
	RenderLightingScene();

	Basement::Renderer::EndScene();

}

void GoofyLandLayer::RenderImGui()
{
	ImGui::Begin("Hello World");

	ImGui::End();
}

void GoofyLandLayer::HandleEvent(Basement::Event& event)
{
	m_CameraController.HandleEvent(event);
}
