#include "cs560Layer.h"

#include "Basement/Renderer/Renderer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "GLFW/glfw3.h"

#include <assimp/types.h>

#include <ImGui/imgui.h>


// floor
float FloorSize = 20.0f;
float FloorLevel = 0.0f;

//glm::vec3 LightPosition(0.0f, 0.0f, 2.0f);
//// Material Shader variables
//float AmbientIntensity = 0.7f;
//float DiffuseIntensity = 0.7f;
//float SpecularIntensity = 1.0f;
//float Shininess = 32.0f;
//float RotationSpeed = 0.3f;
//// Post-Processing
//static int mode = 0;

cs560Layer::cs560Layer()
	: Layer("cs560 Layer"),
	m_CameraController(glm::vec3(0.0f, 10.0f, 25.0f), 45.0f, 1.7778f, 0.1f, 1000.0f)
{
	Basement::Renderer::EnableDepthTest();

	/*m_CameraController = std::make_shared<Basement::CameraController3D>(glm::vec3(0.0f, 10.0f, 25.0f), 45.0f, 1.7778f, 0.1f, 1000.0f);*/

	BuildScene();
}

void cs560Layer::Update(const Basement::Timer& dt)
{

	// Update
	m_CameraController.Update(dt);

	//Basement::Renderer::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	//Basement::Renderer::ClearBufferBit(Basement::RendererGL::ColorBufferBit | Basement::RendererGL::DepthBufferBit);

	Basement::Renderer::BeginScene(m_CameraController.GetCamera());

	RenderScene();

	Basement::Renderer::EndScene();
}

void cs560Layer::RenderImGui()
{
	ImGui::Begin("Scene");

	if (ImGui::CollapsingHeader("Floor")) {
		ImGui::SliderFloat("Size", &FloorSize, 0.0f, 50.0f, "%f", 1.0f);
	}

	ImGui::End();
}

void cs560Layer::HandleEvent(Basement::Event& event)
{
	m_CameraController.HandleEvent(event);
}

void cs560Layer::BuildScene()
{
	//auto& nanoShader = m_ShaderLibrary.Load("assets/shaders/NanoSuit.glsl");
	auto& floorShader = m_ShaderLibrary.Load("assets/shaders/Floor.glsl");
	auto& skyboxShader = m_ShaderLibrary.Load("assets/shaders/Skybox.glsl");
	auto& screenShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuad.glsl");

	m_FloorTexture = Basement::Texture2D::Create("assets/textures/wood.png", true);
	m_SkyboxTexture = Basement::TextureCube::Create("assets/skybox/lake", "jpg");

	//----------------
	// Model
	//----------------
	//m_NanoSuit = Basement::Model::Create("assets/models/nanosuit/nanosuit.obj");


	//----------------
	// Floor
	//----------------
	float planeVertices[] = {
		// note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat
		// positions          // texture Coords
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	m_FloorVAO = Basement::VertexArray::Create();
	m_FloorVBO = Basement::VertexBuffer::Create(sizeof(planeVertices), planeVertices);
	Basement::BufferLayout bufferLayout = {
		{ Basement::EShaderDataType::Float3, "a_Position" },
		{ Basement::EShaderDataType::Float2, "a_TexCood" }
	};
	m_FloorVBO->SetLayout(bufferLayout);
	m_FloorVAO->AddVertexBuffer(m_FloorVBO);

	floorShader->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(floorShader)->UploadUniform1i("u_Texture", 0);


	//----------------
	// Skybox
	//----------------
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	m_SkyboxVAO = Basement::VertexArray::Create();
	m_SkyboxVBO = Basement::VertexBuffer::Create(sizeof(skyboxVertices), skyboxVertices);
	Basement::BufferLayout skyboxLayout = {
		{ Basement::EShaderDataType::Float3, "a_Position"}
	};
	m_SkyboxVBO->SetLayout(skyboxLayout);
	m_SkyboxVAO->AddVertexBuffer(m_SkyboxVBO);

	skyboxShader->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(skyboxShader)->UploadUniform1i("u_SkyboxTexture", 0);


	//----------------
	// Screen Quad
	//----------------
	float quadVertices[] = {
		// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	m_ScreenVAO = Basement::VertexArray::Create();
	m_ScreenVBO = Basement::VertexBuffer::Create(sizeof(quadVertices), quadVertices);
	Basement::BufferLayout screenLayout = {
		{ Basement::EShaderDataType::Float2, "a_Position" },
		{ Basement::EShaderDataType::Float2, "a_TexCoord" }
	};
	m_ScreenVBO->SetLayout(screenLayout);
	m_ScreenVAO->AddVertexBuffer(m_ScreenVBO);

	screenShader->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(screenShader)->UploadUniform1i("u_ScreenTexture", 0);

	//-----------------------------
	// Framebuffer configuration
	//-----------------------------
	m_FrameBuffer = Basement::FrameBuffer::Create(1280, 720);
}

void cs560Layer::RenderScene()
{
	//auto& nanoShader = m_ShaderLibrary.Get("NanoSuit");
	auto& floorShader = m_ShaderLibrary.Get("Floor");
	auto& skyboxShader = m_ShaderLibrary.Get("Skybox");
	auto& screenShader = m_ShaderLibrary.Get("ScreenQuad");

	// Render
	m_FrameBuffer->Bind();
	Basement::Renderer::EnableDepthTest();
	Basement::Renderer::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	Basement::Renderer::ClearBufferBit(Basement::RendererGL::ColorBufferBit | Basement::RendererGL::DepthBufferBit);

	//----------------
	// Model
	//----------------

	//glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * RotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(model)));

	//nanoShader->Bind();
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniformMat3("u_NormalMat", normalMat);
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.position", LightPosition);
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.ambient_power", glm::vec3(AmbientIntensity));
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.diffuse_power", glm::vec3(DiffuseIntensity));
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.specular_power", glm::vec3(SpecularIntensity));
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform1f("u_Material.shininess", Shininess);

	//Basement::Renderer::SubmitModel(m_NanoSuit, nanoShader, model);

	////--------------
	//// Draw Floor
	////--------------
	glm::mat4 floorModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, FloorLevel, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f * FloorSize, 0.0f, 0.3f * FloorSize));
	m_FloorTexture->Bind();
	Basement::Renderer::SubmitArrays(floorShader, m_FloorVAO, 0, 6, floorModel);

	//----------------
	// Skybox
	//----------------
	m_SkyboxTexture->Bind();
	glm::mat4 skyboxModel = glm::translate(glm::mat4(1.0f), m_CameraController.GetCamera().GetPosition());
	Basement::Renderer::SubmitArraysForSkybox(skyboxShader, m_SkyboxVAO, 0, 36, skyboxModel);


	// Swich back to default framebuffer
	m_FrameBuffer->Unbind();
	Basement::Renderer::DisableDepthTest();
	Basement::Renderer::ClearBufferBit(Basement::RendererGL::ColorBufferBit);

	//----------------
	// Screen Quad
	//----------------
	m_FrameBuffer->ActivateTexture();
	screenShader->Bind();
	m_ScreenVAO->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


