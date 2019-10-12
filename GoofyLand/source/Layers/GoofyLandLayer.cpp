#include "GoofyLandLayer.h"

#include "Basement/Renderer/Renderer.h"
#include "Basement/Mesh/Sphere.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <assimp/types.h>


#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define OpenGLCall(x) while (glGetError() != GL_NO_ERROR);\
				x;\
				if(GLenum error = glGetError())\
				{\
					std::stringstream ss; ss << std::hex << error;\
					BM_CORE_ERROR("{0}({1}): error 0x0{2}", __FILENAME__, __LINE__, ss.str());\
					__debugbreak();\
				}

// Global Variables
//glm::vec3 LightPosition(1.2f, 1.0f, 2.0f);
glm::vec3 LightPosition(0.0f, 0.0f, 2.0f);
glm::vec3 LightDirection(-0.2f, -1.0f, 0.5f);
glm::vec3 LightColor(1.0f, 1.0f, 1.0f);

// Point Light (attenuation)
float PointLightConstant = 1.0f;
float PointLightLinear = 0.045f;
float PointLightQuadratic = 0.0075f;

// Spotlight
float InnerCutoffAngle = 12.5f;
float OuterCutoffAngle = 17.5f;

// Material Shader variables
glm::vec3 CubePosition(0.0f, 0.0f, 0.0f);
glm::vec3 CubeColor(1.0f, 0.5f, 0.31f);
glm::vec3 Ambient(1.0f, 0.5f, 0.31f);
glm::vec3 Diffuse(1.0f, 0.5f, 0.31f);
glm::vec3 Specular(0.5, 0.5, 0.5);
//float AmbientIntensity = 0.1f;
float AmbientIntensity = 0.7f;
float DiffuseIntensity = 0.7f;
float SpecularIntensity = 1.0f;

// Emerald Shader
glm::vec3 EmeraldCubePosition(0.7f, 1.0f, 0.7f);
glm::vec3 SilverCubePosition(1.5f, 0.0f, 0.0f);

float EmeraldShininess = 32.0f;

float Shininess = 32.0f;
float Degree = 45.0f;
float RotationSpeed = 0.3f;
#define ROTATE glm::radians(Degree)
#define ROTATE_GLFW RotationSpeed * (float)glfwGetTime()

// floor
float FloorSize = 20.0f;
float FloorLevel = 0.0f;

// Post-Processing
static int mode = 0;


GoofyLandLayer::GoofyLandLayer() :
	Layer("GL"),
	m_CameraController(glm::vec3(0.0f, 10.0f, 25.0f), 45.0f, 1.7778f, 0.1f, 1000.0f)
{
	Basement::Renderer::EnableDepthTest();

	//BuildScene();
	//BuildLightingScene();
	//BuildLightingMapScene();
	//BuildModelScene();
	//BuildSkyboxScene();
	BuildFrameBufferScene();
}


void GoofyLandLayer::Update(const Basement::Timer& dt)
{
	// Update
	m_CameraController.Update(dt);

	// Render
	OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));
	Basement::Renderer::EnableDepthTest();
	Basement::Renderer::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	Basement::Renderer::ClearBufferBit(Basement::RendererGL::ColorBufferBit | Basement::RendererGL::DepthBufferBit);

	Basement::Renderer::BeginScene(m_CameraController.GetCamera());

	//RenderScene();
	//RenderLightingScene();
	//RenderLightingMapScene();
	//RenderModelScene();
	//RednerSkyboxScene();
	RenderFrameBufferScene();

	Basement::Renderer::EndScene();

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
	Basement::SharedPtr<Basement::Shader> textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(textureShader)->UploadUniform1i("u_Texture", 0);

	// Texture
	m_BoxTexture = Basement::Texture2D::Create("assets/textures/mario-block.png");


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
	//	Basement::Shared<Basement::Shader> flatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
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
	// Cube
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
	//Basement::Shared<Basement::Shader> lightingShader = m_ShaderLibrary.Load("assets/shaders/Lighting.glsl");
	Basement::SharedPtr<Basement::Shader> materialShader = m_ShaderLibrary.Load("assets/shaders/Material.glsl");
	Basement::SharedPtr<Basement::Shader> emeraldShader = m_ShaderLibrary.Load("assets/shaders/Emerald.glsl");
	Basement::SharedPtr<Basement::Shader> silverShader = m_ShaderLibrary.Load("assets/shaders/Silver.glsl");

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
	Basement::SharedPtr<Basement::Shader> lightSourceShader = m_ShaderLibrary.Load("assets/shaders/LightSource.glsl");
}

void GoofyLandLayer::RenderLightingScene()
{
	//auto lightingShader = m_ShaderLibrary.Get("Lighting");
	auto lightSourceShader = m_ShaderLibrary.Get("LightSource");
	auto materialShader = m_ShaderLibrary.Get("Material");
	auto emeraldShader = m_ShaderLibrary.Get("Emerald");
	auto silverShader = m_ShaderLibrary.Get("Silver");
	
	// Update uniforms to shaders
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightSourceShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightSourceShader)->UploadUniform3f("u_LightColor", LightColor);

	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("material.ambient", Ambient);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("material.diffuse", Diffuse);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("material.specular", Specular);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform1f("material.shininess", Shininess);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("u_LightColor", LightColor);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("u_LightPosition", LightPosition);
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("light.ambient", AmbientIntensity);
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("light.diffuse", DiffuseIntensity);
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniform3f("light.specular", SpecularIntensity);

	std::dynamic_pointer_cast<Basement::OpenGLShader>(emeraldShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(emeraldShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(emeraldShader)->UploadUniform3f("u_LightPosition", LightPosition);

	std::dynamic_pointer_cast<Basement::OpenGLShader>(silverShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(silverShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(silverShader)->UploadUniform3f("u_LightPosition", LightPosition);


	// Update model matrix
	glm::mat4 lightSourceModel = glm::translate(glm::mat4(1.0f), LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));
	glm::mat4 cubeModel = glm::translate(glm::mat4(1.0f), CubePosition) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 emraldModel = glm::translate(glm::mat4(1.0f), EmeraldCubePosition) * glm::rotate(glm::mat4(1.0f), (float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 silverModel = glm::translate(glm::mat4(1.0f), SilverCubePosition) * glm::rotate(glm::mat4(1.0f), (float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	
	// Update normal matrix
	glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(cubeModel)));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(materialShader)->UploadUniformMat3("u_NormalMat", normalMat);	

	glm::mat3 emaraldNormalMat = glm::mat3(glm::transpose(glm::inverse(emraldModel)));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(emeraldShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(emeraldShader)->UploadUniformMat3("u_NormalMat", emaraldNormalMat);

	glm::mat3 silverNormalMat = glm::mat3(glm::transpose(glm::inverse(silverModel)));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(silverShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(silverShader)->UploadUniformMat3("u_NormalMat", silverNormalMat);

	// Submit VAO to render
	Basement::Renderer::SubmitArrays(lightSourceShader, m_LightVertexArray, 0, 36, lightSourceModel);
	Basement::Renderer::SubmitArrays(materialShader, m_VertexArray, 0, 36, cubeModel);
	Basement::Renderer::SubmitArrays(emeraldShader, m_VertexArray, 0, 36, emraldModel);
	Basement::Renderer::SubmitArrays(silverShader, m_VertexArray, 0, 36, silverModel);
}

void GoofyLandLayer::BuildLightingMapScene()
{
	float lightMappingVertices[] = {
		// Positions          // Normals           // Texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

	//----------------------------
	// Cube
	//----------------------------

	// VAO
	m_VertexArray = Basement::VertexArray::Create();

	// VBO
	m_VertexBuffer = Basement::VertexBuffer::Create(sizeof(lightMappingVertices), lightMappingVertices);
	Basement::BufferLayout bufferLayout = {
		{Basement::EShaderDataType::Float3, "a_Position" },
		{Basement::EShaderDataType::Float3, "a_Normal" },
		{Basement::EShaderDataType::Float2, "a_TexCoord" }
	};
	m_VertexBuffer->SetLayout(bufferLayout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	// Shader
	//Basement::Shared<Basement::Shader> lightingMapShader = m_ShaderLibrary.Load("assets/shaders/LightingMap.glsl");
	Basement::SharedPtr<Basement::Shader> lightingMapShader = m_ShaderLibrary.Load("assets/shaders/LightingMapSpotlight.glsl");
	lightingMapShader->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1i("material.diffuse", 0);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1i("material.specular", 1);
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1i("material.emission", 2);

	// Texture
	m_BoxTexture = Basement::Texture2D::Create("assets/textures/mario-block.png");
	m_BoxSpecularTexture = Basement::Texture2D::Create("assets/textures/mario-block-specular.png");
	//m_BoxTexture = Basement::Texture2D::Create("assets/textures/screen.png");
	//m_BoxSpecularTexture = Basement::Texture2D::Create("assets/textures/container2_specular.png");
	//m_BoxEmissionTexture = Basement::Texture2D::Create("assets/textures/matrix.jpg");
	//m_BoxEmissionTexture = Basement::Texture2D::Create("assets/textures/wall.jpg");


	//----------------------------
	// Light
	//----------------------------

	m_LightVertexArray = Basement::VertexArray::Create();

	m_LightVertexBuffer = Basement::VertexBuffer::Create(sizeof(lightVertices), lightVertices);
	Basement::BufferLayout lightingLayout = {
		{ Basement::EShaderDataType::Float3, "a_Position"}
	};
	m_LightVertexBuffer->SetLayout(lightingLayout);
	m_LightVertexArray->AddVertexBuffer(m_LightVertexBuffer);

	Basement::SharedPtr<Basement::Shader> lightSourceShader = m_ShaderLibrary.Load("assets/shaders/LightSource.glsl");

	// Model
	//m_Model = Basement::Model::Create("resouces/models/nanosuit/nanosuit.obj");


}

void GoofyLandLayer::RenderLightingMapScene()
{
	// Draw light source
	auto& lightSourceShader = m_ShaderLibrary.Get("LightSource");
	
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightSourceShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightSourceShader)->UploadUniform3f("u_LightColor", LightColor);
	//glm::mat4 lightSourceModel = glm::translate(glm::mat4(1.0f), LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));
	glm::mat4 lightSourceModel = glm::translate(glm::mat4(1.0f), m_CameraController.GetCamera().GetPosition() + LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));

	Basement::Renderer::SubmitArrays(lightSourceShader, m_LightVertexArray, 0, 36, lightSourceModel);
	
	
	// Draw cube
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

	auto& lightingMapShader = m_ShaderLibrary.Get("LightingMapSpotlight");
	
	// Upload uniforms
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1f("material.shininess", Shininess);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform3f("light.position", m_CameraController.GetCamera().GetPosition() + LightPosition);
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform3f("light.direction", LightDirection);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform3f("light.direction", m_CameraController.GetCamera().GetFront());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1f("light.innerCutoff", glm::cos(glm::radians(InnerCutoffAngle)));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1f("light.outerCutoff", glm::cos(glm::radians(OuterCutoffAngle)));

	//// PointLight
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1f("light.constant", PointLightConstant);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1f("light.linear", PointLightLinear);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1f("light.quadratic", PointLightQuadratic);

	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform3f("light.ambient", glm::vec3(AmbientIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform3f("light.diffuse", glm::vec3(DiffuseIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform3f("light.specular", glm::vec3(SpecularIntensity));
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniform1f("u_Time", 0.2f * (float)glfwGetTime());

	// Model matrix
	//glm::mat4 cubeModel = glm::translate(glm::mat4(1.0f), CubePosition) * glm::rotate(glm::mat4(1.0f), ROTATE_GLFW, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 cubeModel = glm::mat4(1.0f);
	for (int i = 0; i < 10; ++i)
	{
		cubeModel = glm::translate(glm::mat4(1.0f), cubePositions[i]) *
				glm::rotate(glm::mat4(1.0f), glm::radians(ROTATE_GLFW * i * 20.0f), glm::vec3(1.0f, 0.3f, 0.5f));

		// Normal matrix
		glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(cubeModel)));
		std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->Bind();
		std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniformMat3("u_NormalMat", normalMat);

		// Draw call
		m_BoxTexture->Bind(0);
		m_BoxSpecularTexture->Bind(1);
		Basement::Renderer::SubmitArrays(lightingMapShader, m_VertexArray, 0, 36, cubeModel);
	}

	//// Normal matrix
	//glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(cubeModel)));
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->Bind();
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(lightingMapShader)->UploadUniformMat3("u_NormalMat", normalMat);

	//// Drawcall
	//m_BoxTexture->Bind(0);
	//m_BoxSpecularTexture->Bind(1);
	////m_BoxEmissionTexture->Bind(2);
	//Basement::Renderer::SubmitArrays(lightingMapShader, m_VertexArray, 0, 36, cubeModel);
}

void GoofyLandLayer::BuildModelScene()
{
	Basement::Renderer::EnableStencilTest();
	Basement::Renderer::SetStencilPredicate(Basement::RendererGL::NotEqual, 1, 0xFF);
	Basement::RenderCommand::SetStencilOpSeperate(Basement::RendererGL::Front, Basement::RendererGL::Keep, Basement::RendererGL::Keep, Basement::RendererGL::Replace);
	Basement::RenderCommand::SetStencilOpSeperate(Basement::RendererGL::Back, Basement::RendererGL::Keep, Basement::RendererGL::Replace, Basement::RendererGL::Keep);

	auto& nanoShader = m_ShaderLibrary.Load("assets/shaders/NanoSuit.glsl");
	auto& outlineShader = m_ShaderLibrary.Load("assets/shaders/Outline.glsl");
	auto& floorShader = m_ShaderLibrary.Load("assets/shaders/Floor.glsl");
	auto& skyboxShader = m_ShaderLibrary.Load("assets/shaders/Skybox.glsl");

	//----------------
	// Model
	//----------------
	m_NanoSuit = Basement::Model::Create("assets/models/nanosuit/nanosuit.obj");


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
	m_FloorTexture = Basement::Texture2D::Create("assets/textures/wall.jpg", true);
}

void GoofyLandLayer::RenderModelScene()
{
	auto& nanoShader = m_ShaderLibrary.Get("NanoSuit");
	auto& outlineShader = m_ShaderLibrary.Get("Outline");
	auto& floorShader = m_ShaderLibrary.Get("Floor");

	// model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * RotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	// normal matrix
	glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(model)));

	nanoShader->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniformMat3("u_NormalMat", normalMat);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.position", LightPosition);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.ambient_power", glm::vec3(AmbientIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.diffuse_power", glm::vec3(DiffuseIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.specular_power", glm::vec3(SpecularIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform1f("u_Material.shininess", Shininess);


	//--------------
	// Draw Floor
	//--------------
	Basement::Renderer::EnableStencilMaskOverwrite();
	glm::mat4 floorModel = glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, FloorLevel, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(FloorSize, 0.0f, FloorSize));
	floorShader->Bind();
	m_FloorTexture->Bind(0);
	Basement::Renderer::SubmitArrays(floorShader, m_FloorVAO, 0, 6, floorModel);


	//--------------
	// Draw 3D Model
	//--------------
	// 1. draw model
	Basement::Renderer::SetStencilPredicate(Basement::RendererGL::Always, 1, 0xFF);
	Basement::Renderer::DisableStencilMaskOverwrite();

	Basement::Renderer::SubmitModel(m_NanoSuit, nanoShader, model);

	// 2. draw outline
	Basement::Renderer::SetStencilPredicate(Basement::RendererGL::NotEqual, 1, 0xFF);
	Basement::Renderer::EnableStencilMaskOverwrite();
	Basement::Renderer::DisableDepthTest();
	
	m_NanoSuit->DrawOutline(outlineShader, model);

	Basement::Renderer::DisableStencilMaskOverwrite();
	Basement::Renderer::EnableDepthTest();
}

void GoofyLandLayer::BuildSkyboxScene()
{
	auto& nanoShader = m_ShaderLibrary.Load("assets/shaders/NanoSuit.glsl");
	auto& floorShader = m_ShaderLibrary.Load("assets/shaders/Floor.glsl");
	auto& skyboxShader = m_ShaderLibrary.Load("assets/shaders/Skybox.glsl");

	//----------------
	// Model
	//----------------
	m_NanoSuit = Basement::Model::Create("assets/models/nanosuit/nanosuit.obj");


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
	m_FloorTexture = Basement::Texture2D::Create("assets/textures/wall.jpg", true);


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
	m_SkyboxTexture = Basement::TextureCube::Create("assets/skybox/lake", "jpg");
	//m_SkyboxTexture = Basement::TextureCube::Create("assets/skybox/darkskies", "tga");
}

void GoofyLandLayer::RednerSkyboxScene()
{
	auto& nanoShader = m_ShaderLibrary.Get("NanoSuit");
	auto& floorShader = m_ShaderLibrary.Get("Floor");
	auto& skyboxShader = m_ShaderLibrary.Get("Skybox");

	//----------------
	// Model
	//----------------
	// model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * RotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	// normal matrix
	glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(model)));

	nanoShader->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniformMat3("u_NormalMat", normalMat);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.position", LightPosition);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.ambient_power", glm::vec3(AmbientIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.diffuse_power", glm::vec3(DiffuseIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.specular_power", glm::vec3(SpecularIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform1f("u_Material.shininess", Shininess);

	Basement::Renderer::SubmitModel(m_NanoSuit, nanoShader, model);

	//--------------
	// Draw Floor
	//--------------
	glm::mat4 floorModel = glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, FloorLevel, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f * FloorSize, 0.0f, 0.3f * FloorSize));
	floorShader->Bind();
	m_FloorTexture->Bind(0);
	Basement::Renderer::SubmitArrays(floorShader, m_FloorVAO, 0, 6, floorModel);

	//----------------
	// Skybox
	//----------------

	skyboxShader->Bind();
	m_SkyboxTexture->Bind();
	glm::mat4 skyboxModel = glm::translate(glm::mat4(1.0f), m_CameraController.GetCamera().GetPosition()) *
		glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
	Basement::Renderer::SubmitArraysForSkybox(skyboxShader, m_SkyboxVAO, 0, 36, skyboxModel);
}

void GoofyLandLayer::BuildFrameBufferScene()
{
	auto& nanoShader = m_ShaderLibrary.Load("assets/shaders/NanoSuit.glsl");
	auto& floorShader = m_ShaderLibrary.Load("assets/shaders/Floor.glsl");
	auto& skyboxShader = m_ShaderLibrary.Load("assets/shaders/Skybox.glsl");
	auto& screenShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuad.glsl");
	auto& inversionShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuadInversion.glsl");
	auto& grayShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuadGrayScale.glsl");
	auto& kernelShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuadKernel.glsl");

	m_FloorTexture = Basement::Texture2D::Create("assets/textures/wood.png", true);
	m_SkyboxTexture = Basement::TextureCube::Create("assets/skybox/lake", "jpg");

	//----------------
	// Model
	//----------------
	m_NanoSuit = Basement::Model::Create("assets/models/nanosuit/nanosuit.obj");


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

	// 1. create fbo
	OpenGLCall(glGenFramebuffers(1, &m_Framebuffer));
	OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));

	// 2. texture attchment
	// 2.1 create texture
	OpenGLCall(glGenTextures(1, &m_TexColorBuffer));
	OpenGLCall(glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer));

	OpenGLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
	OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OpenGLCall(glBindTexture(GL_TEXTURE_2D, 0));
	
	// 2.2 attach texture to framebuffer
	OpenGLCall( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexColorBuffer, 0));

	// 3. renderbuffer attachment
	// 3.1 create renderbuffer object
	OpenGLCall(glGenRenderbuffers(1, &m_RenderBuffer));
	OpenGLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer));
	OpenGLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720));
	OpenGLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	// 3.2 bind depth and stencil attachment
	OpenGLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer));

	// 4. check framebuffer status
	BM_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");
	OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GoofyLandLayer::RenderFrameBufferScene()
{
	auto& nanoShader = m_ShaderLibrary.Get("NanoSuit");
	auto& floorShader = m_ShaderLibrary.Get("Floor");
	auto& skyboxShader = m_ShaderLibrary.Get("Skybox");
	auto& screenShader = m_ShaderLibrary.Get("ScreenQuad");
	auto& inversionShader = m_ShaderLibrary.Get("ScreenQuadInversion");
	auto& grayShader = m_ShaderLibrary.Get("ScreenQuadGrayScale");
	auto& kernelShader = m_ShaderLibrary.Get("ScreenQuadKernel");


	//----------------
	// Model
	//----------------

	glm::mat4 model(1.0f);
	model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * RotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(model)));

	nanoShader->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniformMat3("u_NormalMat", normalMat);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.position", LightPosition);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.ambient_power", glm::vec3(AmbientIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.diffuse_power", glm::vec3(DiffuseIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform3f("u_Light.specular_power", glm::vec3(SpecularIntensity));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(nanoShader)->UploadUniform1f("u_Material.shininess", Shininess);

	//Basement::Renderer::SubmitModel(m_NanoSuit, nanoShader, model);


	//--------------
	// Draw Floor
	//--------------
	glm::mat4 floorModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, FloorLevel, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f * FloorSize, 0.0f, 0.3f * FloorSize));
	m_FloorTexture->Bind(0);
	//Basement::Renderer::SubmitArrays(floorShader, m_FloorVAO, 0, 6, floorModel);

	//----------------
	// Skybox
	//----------------
	m_SkyboxTexture->Bind();
	glm::mat4 skyboxModel = glm::translate(glm::mat4(1.0f), m_CameraController.GetCamera().GetPosition()) * 
		glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * RotationSpeed * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	Basement::Renderer::SubmitArraysForSkybox(skyboxShader, m_SkyboxVAO, 0, 36, skyboxModel);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Basement::Renderer::DisableDepthTest();
	Basement::Renderer::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	Basement::Renderer::ClearBufferBit(Basement::RendererGL::ColorBufferBit);
	//----------------
	// Screen Quad
	//----------------
	screenShader->Bind();
	glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer);
	glm::mat4 screenModel(1.0f);
	
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
	default:
		screenShader->Bind(); break;
	}

	
	
	m_ScreenVAO->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//Basement::Renderer::SubmitArrays(screenShader, m_ScreenVAO, 0, 6, screenModel);
}




void GoofyLandLayer::RenderImGui()
{
	ImGui::Begin("Scene");

	if (ImGui::CollapsingHeader("Mode"))
	{
		ImGui::RadioButton("Normal", &mode, 0);
		ImGui::RadioButton("Inversion", &mode, 1);
		ImGui::RadioButton("Grayscale", &mode, 2);
		ImGui::RadioButton("Kernel", &mode, 3);
	}

	if (ImGui::CollapsingHeader("Light"))
	{
		if (ImGui::TreeNode("Bulb")) {
			ImGui::SliderFloat3("Light Position", glm::value_ptr(LightPosition), -3.0f, 3.0f, "%.1f", 2.0f);
			ImGui::ColorEdit3("Light Color", glm::value_ptr(LightColor));
			ImGui::SliderFloat("Inner Cutoff Angle", &InnerCutoffAngle, 0.0, OuterCutoffAngle, "%.1f", 1.0f);
			ImGui::SliderFloat("Outer Cutoff Angle", &OuterCutoffAngle, 0.0, 30.0f, "%.1f", 1.0f);

			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ImGui::CollapsingHeader("Object")) {
		if (ImGui::TreeNode("Cube")) {
			ImGui::SliderFloat3("Cube Position", glm::value_ptr(CubePosition), -3.0f, 3.0f, "%.1f", 1.0f);
			ImGui::ColorEdit3("Cube Color", glm::value_ptr(CubeColor));
			ImGui::SliderFloat("Rotation", &Degree, 0.0f, 360.0f, "%f", 1.0f);
			ImGui::SliderFloat("Rotation Speed", &RotationSpeed, 0.0f, 1.0f, "%0.1f", 1.0f);
			if (ImGui::TreeNode("Material")) {
				ImGui::SliderFloat("Ambient", &AmbientIntensity, 0.0f, 1.0f, "%.2f", 1.0f);
				ImGui::SliderFloat("Diffuse", &DiffuseIntensity, 0.0f, 1.0f, "%.2f", 1.0f);
				ImGui::SliderFloat("Specular", &SpecularIntensity, 0.0f, 1.0f, "%.2f", 1.0f);
				ImGui::SliderFloat("Shininess", &Shininess, 0, 256, "%f");

				ImGui::TreePop();
			}
			
			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	if (ImGui::CollapsingHeader("Floor")) {
		ImGui::SliderFloat("Size", &FloorSize, 0.0f, 50.0f, "%f", 1.0f);
	}

	ImGui::End();
}

void GoofyLandLayer::HandleEvent(Basement::Event& event)
{
	m_CameraController.HandleEvent(event);
}
