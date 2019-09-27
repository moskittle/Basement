#include "GoofyLandLayer.h"

#include "Basement/Renderer/RenderCommand.h"
#include "Basement/Renderer/Renderer.h"
#include "Basement/Mesh/Sphere.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

// Global Variables
//glm::vec3 LightPosition(1.2f, 1.0f, 2.0f);
glm::vec3 LightPosition(0.7f, 0.0f, 0.6f);
glm::vec3 LightColor(1.0f, 1.0f, 1.0f);

glm::vec3 CubePosition(0.0f, 0.0f, 0.0f);
glm::vec3 CubeColor(1.0f, 0.5f, 0.31f);
glm::vec3& Ambient= CubeColor;
glm::vec3& Diffuse = CubeColor;
glm::vec3 Specular(0.5, 0.5, 0.5);
glm::vec3 AmbientIntensity(0.2f);
glm::vec3 DiffuseIntensity(0.5f);
glm::vec3 SpecularIntensity(1.0f);

int Shininess = 32;

GoofyLandLayer::GoofyLandLayer() : Layer("GL"), m_CameraController(glm::vec3(0.0f, 1.0f, 5.0f), 45.0f, 1.7778f, 0.1f, 100.0f)
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
		// Position			  // Normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
		{ Basement::EShaderDataType::Float3, "a_Normal" }
	};
	m_VertexBuffer->SetLayout(bufferLayout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	// Shader
	//Basement::Shared<Basement::Shader> lightingShader = m_ShaderLibrary.Load("resource/shaders/Lighting.glsl");
	Basement::Shared<Basement::Shader> materialShader = m_ShaderLibrary.Load("resource/shaders/Material.glsl");
	
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

	// Shader
	Basement::Shared<Basement::Shader> lightSourceShader = m_ShaderLibrary.Load("resource/shaders/LightSource.glsl");
}

void GoofyLandLayer::RenderLightingScene()
{
	//auto lightingShader = m_ShaderLibrary.Get("Lighting");
	auto lightSourceShader = m_ShaderLibrary.Get("LightSource");
	auto materialShader = m_ShaderLibrary.Get("Material");
	// Update uniforms to lighting shaders
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("material.ambient", Ambient);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("material.diffuse", Diffuse);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("material.specular", Specular);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform1i("material.shininess", Shininess);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("u_LightColor", LightColor);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("u_LightPosition", LightPosition);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("light.ambient", AmbientIntensity);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("light.diffuse", DiffuseIntensity);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("light.specular", SpecularIntensity);

	// Update uniforms to light source shaders
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightSourceShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightSourceShader)->UploadUniform3f("u_LightColor", LightColor);

	// Update model matrix
	glm::mat4 lightSourceModel = glm::translate(glm::mat4(1.0f), LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
	glm::mat4 cubeModel = glm::translate(glm::mat4(1.0f), CubePosition) * glm::rotate(glm::mat4(1.0f), (float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	// Update normal matrix
	glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(cubeModel)));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniformMat3("u_NormalMat", normalMat);

	// Submit VAO to render
	Basement::Renderer::SubmitArrays(lightSourceShader, m_LightVertexArray, 0, 36, lightSourceModel);
	Basement::Renderer::SubmitArrays(materialShader, m_VertexArray, 0, 36, cubeModel);
}



void GoofyLandLayer::Update(const Basement::Timer& dt)
{
	// Update
	m_CameraController.Update(dt);

	// Render
	Basement::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	//Basement::RenderCommand::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	Basement::RenderCommand::Clear();

	Basement::Renderer::BeginScene(m_CameraController.GetCamera());

	//RenderScene();
	RenderLightingScene();

	Basement::Renderer::EndScene();

}

void GoofyLandLayer::RenderImGui()
{
	ImGui::Begin("Scene");

	if (ImGui::CollapsingHeader("Light"))
	{
		if (ImGui::TreeNode("Bulb")) {
			ImGui::SliderFloat3("Light Position", glm::value_ptr(LightPosition), -3.0f, 3.0f, "%.1f", 2.0f);
			ImGui::ColorEdit3("Light Color", glm::value_ptr(LightColor));

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ImGui::CollapsingHeader("Object")) {
		if (ImGui::TreeNode("Cube")) {
			ImGui::SliderFloat3("Cube Position", glm::value_ptr(CubePosition), -3.0f, 3.0f, "%.1f", 1.0f);
			ImGui::ColorEdit3("Cube Color", glm::value_ptr(CubeColor));
			if (ImGui::TreeNode("Material")) {
				ImGui::SliderFloat3("Ambient", glm::value_ptr(Ambient), 0.0f, 1.0f, "%.2f", 1.0f);
				ImGui::SliderFloat3("Diffuse", glm::value_ptr(Diffuse), 0.0f, 1.0f, "%.2f", 1.0f);
				ImGui::SliderFloat3("Specular", glm::value_ptr(Specular), 0.0f, 1.0f, "%.2f", 1.0f);
				ImGui::SliderInt("Shininess", &Shininess, 0, 256, "%f");
				ImGui::TreePop();
			}
			
			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	ImGui::End();
}

void GoofyLandLayer::HandleEvent(Basement::Event& event)
{
	m_CameraController.HandleEvent(event);
}
