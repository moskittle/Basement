#include "GoofyLandLayer.h"

#include "Basement/Renderer/Renderer.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <GLFW/glfw3.h>

#include <assimp/types.h>

// Global Variables
glm::vec3 LightPosition(0.0f, 0.0f, 2.0f);
glm::vec3 LightDirection(-0.2f, -1.0f, 0.5f);
glm::vec3 LightColor(1.0f, 1.0f, 1.0f);

// Material Shader variables
glm::vec3 Ambient(1.0f, 0.5f, 0.31f);
glm::vec3 Diffuse(1.0f, 0.5f, 0.31f);
glm::vec3 Specular(0.5, 0.5, 0.5);
float AmbientIntensity = 0.7f;
float DiffuseIntensity = 0.7f;
float SpecularIntensity = 1.0f;

float Shininess = 32.0f;
float RotationSpeed = 0.3f;


// Post-Processing
static int mode = 0;


GoofyLandLayer::GoofyLandLayer() :
	Layer("GL"),
	m_CameraController(glm::vec3(0.0f, 3.0f, 10.0f), 45.0f, 1.7778f, 0.1f, 1000.0f)
{
	Basement::Renderer::EnableDepthTest();

	BuildScene();
}


void GoofyLandLayer::Update(const Basement::Timer& dt)
{
	// Update
	m_CameraController.Update(dt);

	//Basement::Renderer::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	//Basement::Renderer::ClearBufferBit(Basement::RendererGL::ColorBufferBit | Basement::RendererGL::DepthBufferBit);

	Basement::Renderer::BeginScene(m_CameraController.GetCamera());

	RenderScene();

	Basement::Renderer::EndScene();

}

void GoofyLandLayer::BuildScene()
{
	auto& nanoShader = m_ShaderLibrary.Load("assets/shaders/NanoSuit.glsl");
	auto& floorShader = m_ShaderLibrary.Load("assets/shaders/Floor.glsl");
	auto& skyboxShader = m_ShaderLibrary.Load("assets/shaders/Skybox.glsl");

	// post-processing shaders
	auto& screenShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuad.glsl");
	auto& inversionShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuadInversion.glsl");
	auto& grayShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuadGrayScale.glsl");
	auto& kernelShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuadKernel.glsl");
	auto& blurShaderShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuadBlur.glsl");

	m_SkyboxTexture = Basement::TextureCube::Create("assets/skybox/lake", "jpg");

	//----------------
	// Model
	//----------------
	m_NanoSuit = Basement::Model::Create("assets/models/nanosuit/nanosuit.obj");

	//----------------
	// Sponza
	//----------------
	m_Sponza = Basement::Model::Create("assets/models/sponza/sponza.obj");

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

void GoofyLandLayer::RenderScene()
{
	auto& nanoShader = m_ShaderLibrary.Get("NanoSuit");
	auto& floorShader = m_ShaderLibrary.Get("Floor");
	auto& skyboxShader = m_ShaderLibrary.Get("Skybox");
	auto& screenShader = m_ShaderLibrary.Get("ScreenQuad");
	auto& inversionShader = m_ShaderLibrary.Get("ScreenQuadInversion");
	auto& grayShader = m_ShaderLibrary.Get("ScreenQuadGrayScale");
	auto& kernelShader = m_ShaderLibrary.Get("ScreenQuadKernel");
	auto& blurShader = m_ShaderLibrary.Get("ScreenQuadBlur");

	// Render
	m_FrameBuffer->Bind();
	Basement::Renderer::EnableDepthTest();
	Basement::Renderer::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	Basement::Renderer::ClearBufferBit(Basement::RendererGL::ColorBufferBit | Basement::RendererGL::DepthBufferBit);

	//----------------
	// Model
	//----------------
	glm::mat4 nanoModel(1.0f);
	nanoModel = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * RotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	glm::mat3 nanoNormalMat = glm::mat3(glm::transpose(glm::inverse(nanoModel)));

	nanoShader->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniformMat3("u_NormalMat", nanoNormalMat);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.position", LightPosition);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.ambient_power", glm::vec3(AmbientIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.diffuse_power", glm::vec3(DiffuseIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.specular_power", glm::vec3(SpecularIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform1f("u_Material.shininess", Shininess);

	Basement::Renderer::SubmitModel(m_NanoSuit, nanoShader, nanoModel);

	//----------------
	// Sponza
	//----------------
	glm::mat4 sponzaModelMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	glm::mat3 sponzaNormalMat = glm::mat3(glm::transpose(glm::inverse(sponzaModelMat)));

	nanoShader->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniformMat3("u_NormalMat", sponzaNormalMat);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.position", LightPosition);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.ambient_power", glm::vec3(AmbientIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.diffuse_power", glm::vec3(DiffuseIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.specular_power", glm::vec3(SpecularIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform1f("u_Material.shininess", Shininess);

	Basement::Renderer::SubmitModel(m_Sponza, nanoShader, sponzaModelMat);

	//----------------
	// Skybox
	//----------------
	m_SkyboxTexture->Bind();
	glm::mat4 skyboxModel = glm::translate(glm::mat4(1.0f), m_CameraController.GetCamera().GetPosition()) /**
		glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * RotationSpeed * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f))*/;
	Basement::Renderer::SubmitArraysForSkybox(skyboxShader, m_SkyboxVAO, 0, 36, skyboxModel);


	// Swich back to default framebuffer
	m_FrameBuffer->Unbind();
	Basement::Renderer::DisableDepthTest();
	//Basement::Renderer::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	Basement::Renderer::ClearBufferBit(Basement::RendererGL::ColorBufferBit);

	//----------------
	// Screen Quad
	//----------------
	m_FrameBuffer->ActivateTexture();

	// large
	switch (mode)
	{
	case 0:
		screenShader->Bind(); break;
	case 1:
		inversionShader->Bind(); break;
	case 2:
		grayShader->Bind(); break;
	case 3:
		kernelShader->Bind(); break;
	case 4:
		blurShader->Bind(); break;
	default:
		screenShader->Bind(); break;
	}

	m_ScreenVAO->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GoofyLandLayer::RenderImGui()
{
	ImGui::Begin("Scene");

	if (ImGui::CollapsingHeader("Scene")) {
		if (ImGui::TreeNode("NanoSuit")) {
			ImGui::SliderFloat("Rotation Speed", &RotationSpeed, 0.0f, 1.0f, "%0.1f", 1.0f);

			ImGui::TreePop();
			ImGui::Separator();
		}

		if (ImGui::TreeNode("Bulb")) {
			ImGui::SliderFloat3("Light Position", glm::value_ptr(LightPosition), -10.0f, 10.0f, "%.1f", 2.0f);
			ImGui::ColorEdit3("Light Color", glm::value_ptr(LightColor));

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ImGui::CollapsingHeader("PostProcessing"))
	{
		ImGui::RadioButton("Normal", &mode, 0);
		ImGui::RadioButton("Inversion", &mode, 1);
		ImGui::RadioButton("Grayscale", &mode, 2);
		ImGui::RadioButton("Kernel", &mode, 3);
		ImGui::RadioButton("Blur", &mode, 4);
	}

	ImGui::End();
}

void GoofyLandLayer::HandleEvent(Basement::Event& event)
{
	m_CameraController.HandleEvent(event);
}
