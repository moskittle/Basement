#include "cs560Layer.h"

#include "Basement/Renderer/Renderer.h"
#include "Basement/Renderer/Path/SpeedControl.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <assimp/types.h>
#include <ImGui/imgui.h>

float FloorSize = 1.0f;
float FloorPositionY = 0.5f;

bool showPath = true;
bool showControlPoints = false;
bool showModel = false;
bool showJoints = true;
bool showBones = true;

float elapsingTime = 0.0f;
float totalTime = 12.0f;
float arcValue = 0.0f;
float angle = 0.0f;
glm::vec3 forwardDirection = glm::vec3(0.0f, 0.0f, 1.0f);
float animationPace = 8.0f;

glm::vec3 cubePosition = glm::vec3(1.0f, 1.0f, 3.0f);
glm::vec3 ballPosition = glm::vec3(1.0f, 1.0f, 3.0f);
glm::vec3 clothPosition = glm::vec3(1.0f, 1.0f, 3.0f);
glm::vec3 modelPosition = glm::vec3(0.0f, 0.5f, 1.5f);

float ballRadius = 0.5f;
float ballAmbientIntensity = 1.0f;
glm::vec3 ballBaseColor = glm::vec3(0.4f, 0.4f, 0.4f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPosition = glm::vec3(0.0f, 2.0f, 10.0f);


std::vector<glm::vec3> pathPoints = {
	glm::vec3(-4.0f, 0.0f, -4.0f),
	glm::vec3(-1.4f, 0.0f, -3.4f),
	glm::vec3(-0.8f,  0.0f, -2.8f),
	glm::vec3(2.0f,  0.0f, -1.8f),
	glm::vec3(-0.1f, 0.0f, 0.8f),
	glm::vec3(-3.1f, 0.0f, -1.1f),
	glm::vec3(-0.2f, 0.0f, 0.6f),
	glm::vec3(-4.2f, 0.0f, 0.5f),
	glm::vec3(-1.6f,  0.0f, 3.0f),
	glm::vec3(0.0f,   0.0f, 1.8f),
	glm::vec3(1.9f,  0.0f, 3.8f),
	glm::vec3(3.1f,  0.0f, 4.0f),
};

cs560Layer::cs560Layer()
	: Layer("cs560 Layer"),
	m_CameraController(glm::vec3(2.0f, 3.0f, 10.0f), 45.0f, 1.7778f, 0.1f, 1000.0f)
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

	RenderScene(dt);

	Basement::Renderer::EndScene();
}

void cs560Layer::RenderImGui()
{
	ImGui::Begin("Scene");

	if (ImGui::CollapsingHeader("Instruction"))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Input:");
		ImGui::Indent();
		ImGui::BulletText("Movement: Use WSAD to move the camera.");
		ImGui::BulletText("Look Around: Hold Right Mouse Button to look around.");
		ImGui::Unindent();
		ImGui::Separator();
	}

	//if (ImGui::CollapsingHeader("Animation"))
	//{
	//	ImGui::Checkbox("Show Model", &showModel);
	//	ImGui::Checkbox("Show Joints", &showJoints);
	//	ImGui::Checkbox("Show Bones", &showBones);
	//}

	//if (ImGui::CollapsingHeader("Path"))
	//{
	//	ImGui::Checkbox("Show Path", &showPath);
	//	ImGui::Separator();
	//	ImGui::Checkbox("Show Control Points", &showControlPoints);

	//	//if (ImGui::TreeNode("Control Points")) {
	//	//	int index = 0;
	//	//	for (auto& controlPoint : pathPoints)
	//	//	{
	//	//		std::string name = "Point " + std::to_string(index);
	//	//		ImGui::SliderFloat3(name.c_str(), glm::value_ptr(controlPoint), -10.0f, 10.0f, "%.1f", 2.0f);
	//	//		index++;
	//	//	}
	//	//	ImGui::TreePop();
	//	//}
	//}

	//if (ImGui::CollapsingHeader("Cube"))
	//{
	//	ImGui::SliderFloat3("Cube Position", glm::value_ptr(cubePosition), 0.0f, 3.5f, "%.01f", 1.0f);
	//}

	if (ImGui::CollapsingHeader("Cloth Simulation"))
	{
		ImGui::SliderFloat3("Ball Position", glm::value_ptr(ballPosition), 0.0f, 4.0f, "%.3f", 1.0f);
		ImGui::SliderFloat("Ball Radius", &ballRadius, 0.0f, 1.0f, "%.3f", 0.5f);
		//ImGui::SliderFloat("Ball Ambient Intensity", &ballAmbientIntensity, 0.0f, 1.0f, "%.3f", 1.0f);
		//ImGui::ColorPicker3("Ball Base Color", glm::value_ptr(ballBaseColor));
		//ImGui::ColorPicker3("Light Color", glm::value_ptr(lightColor));
		//ImGui::SliderFloat3("Light Position", glm::value_ptr(lightPosition), 0.0f, 4.0f, "%.1f", 1.0f);
	}

	ImGui::Text("FPS: %.1f FPS", ImGui::GetIO().Framerate);

	ImGui::Separator();
	ImGui::End();
}

void cs560Layer::HandleEvent(Basement::Event& event)
{
	m_CameraController.HandleEvent(event);
}

void cs560Layer::BuildScene()
{
	auto& flatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
	auto& simpleLitShader = m_ShaderLibrary.Load("assets/shaders/SimpleLit.glsl");
	auto& simpleLitClothShader = m_ShaderLibrary.Load("assets/shaders/SimpleLitCloth.glsl");
	auto& animationShader = m_ShaderLibrary.Load("assets/shaders/SkeletonAnimation.glsl");
	auto& animationNoTexShader = m_ShaderLibrary.Load("assets/shaders/SkeletonAnimationNoTex.glsl");
	auto& boneShader = m_ShaderLibrary.Load("assets/shaders/DebugBone.glsl");
	auto& lineShader = m_ShaderLibrary.Load("assets/shaders/DebugLine.glsl");
	auto& floorShader = m_ShaderLibrary.Load("assets/shaders/Floor.glsl");
	auto& skyboxShader = m_ShaderLibrary.Load("assets/shaders/Skybox.glsl");
	auto& screenShader = m_ShaderLibrary.Load("assets/shaders/ScreenQuad.glsl");

	m_DoozyDiffuseTex = Basement::Texture2D::Create("assets/models/doozy/doozy_diffuse.png", false);
	m_FloorTexture = Basement::Texture2D::Create("assets/textures/wood.png", true);
	m_SkyboxTexture = Basement::TextureCube::Create("assets/skybox/lake", "jpg");


	//----------------
	// Model
	//----------------

	//// Doozy setup
	//m_Doozy = Basement::Model::Create("assets/models/doozy/doozy.fbx");
	//std::unordered_map<std::string, Basement::SharedPtr<Basement::Animation>> doozyAnimationLibrary;
	//auto walkAnimation = std::make_shared<Basement::Animation>("assets/models/doozy/Walking.fbx", m_Doozy);
	//auto slowRunAnimation = std::make_shared<Basement::Animation>("assets/models/doozy/SlowRun.fbx", m_Doozy);
	//doozyAnimationLibrary["Walking"] = walkAnimation;
	//doozyAnimationLibrary["SlowRun"] = slowRunAnimation;
	//m_DoozyAnimator = std::make_shared<Basement::Animator>(doozyAnimationLibrary);
	//m_DoozyAnimator->PlayAnimation("Walking");
	//m_DoozyAnimator->GenerateInverseKinematicsData("mixamorig1:LeftHandIndex2");

	// Stamp setup
	m_Stamp = Basement::Model::Create("assets/models/stamp.x");
	std::unordered_map<std::string, Basement::SharedPtr<Basement::Animation>> stampAnimationLibrary;
	auto stampWalkAnimation = std::make_shared<Basement::Animation>("assets/models/stamp.x", m_Stamp);
	stampAnimationLibrary["Walking"] = stampWalkAnimation;
	m_StampAnimator = std::make_shared<Basement::Animator>(stampAnimationLibrary);
	m_StampAnimator->PlayAnimation("Walking");
	//m_StampAnimator->GenerateInverseKinematicsData("Bip01_R_Finger02");
	m_StampAnimator->GenerateInverseKinematicsData("Bip01_L_Finger12");
	//m_StampAnimator->GenerateInverseKinematicsData("Bip01_L_Finger1");

	//----------------
	// Cube & Ball
	//----------------
	m_Cube = Basement::Model::Create("assets/models/cube.fbx");
	m_Ball = Basement::Model::Create("assets/models/sphere.fbx");
	//m_Cloth = std::make_shared<Basement::Cloth>(3.0f, 2.0f, 50, 40);
	m_Cloth = std::make_shared<Basement::Cloth>(3.0f, 2.0f, 30, 30);
	//m_Cloth = std::make_shared<Basement::Cloth>(14, 10, 55, 45);

	//----------------
	// Path
	//----------------
	m_Path = Basement::Path(pathPoints);

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

void cs560Layer::RenderScene(const Basement::Timer& dt)
{
	auto& boneShader = m_ShaderLibrary.Get("DebugBone");
	auto& lineShader = m_ShaderLibrary.Get("DebugLine");
	auto& animationShader = m_ShaderLibrary.Get("SkeletonAnimation");
	auto& animationNoTexShader = m_ShaderLibrary.Get("SkeletonAnimationNoTex");
	auto& floorShader = m_ShaderLibrary.Get("Floor");
	auto& skyboxShader = m_ShaderLibrary.Get("Skybox");
	auto& screenShader = m_ShaderLibrary.Get("ScreenQuad");
	auto& flatColorShader = m_ShaderLibrary.Get("FlatColor");
	auto& simpleLitShader = m_ShaderLibrary.Get("SimpleLit");
	auto& simpleLitClothShader = m_ShaderLibrary.Get("SimpleLitCloth");

	// Render
	m_FrameBuffer->Bind();
	Basement::Renderer::EnableDepthTest();
	Basement::Renderer::SetClearColor(glm::vec4(0.8f, 0.6f, 0.8f, 1.0f));
	Basement::Renderer::ClearBufferBit(Basement::RendererGL::ColorBufferBit | Basement::RendererGL::DepthBufferBit);


	// -------------------------------------------------------------
	// --------Project 2--------------------------------------------
	// -------------------------------------------------------------

	//----------------
	// Path
	//----------------
	//glm::mat4 pathModelMat = glm::mat4(1.0f);
	//m_Path.Draw(lineShader, pathModelMat, showPath, showControlPoints);

	//m_ArcLength = Basement::ArcLength(m_Path.GetPointsOnCurve());
	//Basement::SpeedControl speedControl;
	//speedControl.SetEaseInInterval(0.0f, 3.0f);
	//speedControl.SetEaseOutInterval(totalTime - 3.0f, totalTime);
	//speedControl.CalculateMaxSpeed();

	//// Update Position and orientation
	//elapsingTime += dt;
	//elapsingTime = elapsingTime >= totalTime ? 0.0f : elapsingTime;
	//arcValue = speedControl.CalculateCurrentPosition(elapsingTime);
	//float curArcPos = m_ArcLength.GetValueFromLookUpTable(arcValue);
	//float nextArcPos = glm::clamp(curArcPos + dt, curArcPos + dt, 1.0f);
	//glm::vec3 curPos = m_Path.CalculatePointPostion(curArcPos);
	//glm::vec3 nextPos = m_Path.CalculatePointPostion(nextArcPos);
	//angle = nextArcPos < 1.0f ? glm::acos(glm::dot(forwardDirection, facingDir)) : angle;
	//angle = glm::cross(forwardDirection, facingDir).y < 0 ? angle * -1.0f : angle;


	////----------------
	//// Model
	////----------------
	//glm::vec3 moveDir = cubePosition - modelPosition;
	//glm::vec3 facingDir = glm::normalize(moveDir);
	//angle = glm::acos(glm::dot(forwardDirection, facingDir));
	//angle = glm::cross(forwardDirection, facingDir).y < 0 ? angle * -1.0f : angle;
	//float distance = glm::length(moveDir);

	//if (distance > 0.35f)
	//{
	//	float relativeSpeed = glm::clamp(static_cast<float>(dt), 0.0f, 0.016f);
	//	modelPosition += (moveDir * relativeSpeed);
	//	m_DoozyAnimator->UpdateAnimation(dt);
	//}
	//else
	//{
	//	// Do Inverse Kinematics
	//}

	//glm::mat4 model = glm::translate(glm::mat4(1.0f), modelPosition)
	//	* glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f))
	//	* glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));


	//auto boneMatrices = m_DoozyAnimator->GetFinalBoneMatrices();

	//if (showModel)
	//{
	//	animationShader->Bind();
	//	for (int i = 0; i < 100; ++i)
	//	{
	//		std::dynamic_pointer_cast<Basement::OpenGLShader>(animationShader)->UploadUniformMat4("u_FinalBoneMatrices[" + std::to_string(i) + "]", boneMatrices[i]);
	//	}
	//	animationShader->Unbind();
	//	m_DoozyDiffuseTex->Bind();
	//	Basement::Renderer::SubmitModel(m_Doozy, animationShader, model);
	//}

	//// Draw bone
	//boneShader->Bind();
	//for (int i = 0; i < 100; ++i)
	//{
	//	std::dynamic_pointer_cast<Basement::OpenGLShader>(animationShader)->UploadUniformMat4("u_FinalBoneMatrices[" + std::to_string(i) + "]", boneMatrices[i]);
	//}
	//boneShader->Unbind();
	//m_DoozyAnimator->DrawSkeleton(boneShader, model, showJoints, showBones);


	// -------------------------------------------------------------
	// --------Project 3--------------------------------------------
	// -------------------------------------------------------------

	//// Draw Stamp
	//glm::mat4 stampModelMatrix = glm::translate(glm::mat4(1.0f), modelPosition)
	//	* glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f))
	//	* glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

	//glm::vec3 moveDir = cubePosition - modelPosition;
	//moveDir.y = 0.0f;
	//glm::vec3 facingDir = glm::normalize(moveDir);
	//angle = glm::acos(glm::dot(forwardDirection, facingDir));
	//angle = glm::cross(forwardDirection, facingDir).y < 0 ? angle * -1.0f : angle;
	//float distance = glm::length(moveDir);

	//if (distance > 0.35f)
	//{
	//	float relativeSpeed = glm::clamp(static_cast<float>(dt), 0.0f, 0.016f);
	//	modelPosition += (moveDir * relativeSpeed);
	//	auto animationPace = glm::clamp((dt / 0.016f), 0.0f, 1.0f);
	//	m_StampAnimator->UpdateAnimation(animationPace * dt);
	//}
	//else
	//{
	//	glm::vec3 targetPosition = glm::inverse(stampModelMatrix) * glm::vec4(cubePosition, 1.0f);
	//	// Do Inverse Kinematics
	//	m_StampAnimator->SolveInverseKinematicsCCD(targetPosition, dt);
	//}

	//auto stampBoneMatrices = m_StampAnimator->GetFinalBoneMatrices();

	////if (showModel)
	////{
	////	animationNoTexShader->Bind();
	////	for (int i = 0; i < 100; ++i)
	////	{
	////		std::dynamic_pointer_cast<Basement::OpenGLShader>(animationShader)->UploadUniformMat4("u_FinalBoneMatrices[" + std::to_string(i) + "]", stampBoneMatrices[i]);
	////	}
	////	animationNoTexShader->Unbind();
	////	Basement::Renderer::SubmitModel(m_Stamp, animationNoTexShader, stampModelMatrix);
	////}

	//// Draw bone
	//boneShader->Bind();
	//for (int i = 0; i < 100; ++i)
	//{
	//	std::dynamic_pointer_cast<Basement::OpenGLShader>(animationShader)->UploadUniformMat4("u_FinalBoneMatrices[" + std::to_string(i) + "]", stampBoneMatrices[i]);
	//}
	//boneShader->Unbind();
	//m_StampAnimator->DrawSkeleton(boneShader, stampModelMatrix, showJoints, showBones);

	////--------------
	//// Draw Cube
	////--------------
	//glm::mat4 cubeModelMat = glm::translate(glm::mat4(1.0f), cubePosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));
	//m_Cube->Draw(flatColorShader, cubeModelMat);

	//--------------
	// Draw Ball
	//--------------

	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitShader)->UploadUniform1f("u_AmbientIntensity", ballAmbientIntensity);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitShader)->UploadUniform3f("u_BaseColor", ballBaseColor);
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitShader)->UploadUniform3f("u_ViewPosition", m_CameraController.GetCamera().GetPosition());
	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitShader)->UploadUniform3f("u_LightColor", lightColor);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitShader)->UploadUniform3f("u_LightPosition", lightPosition);
	//std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitShader)->Unbind();


	glm::mat4 ballModelMatrix = glm::translate(glm::mat4(1.0f), ballPosition);
	m_Ball->Draw(simpleLitShader, ballModelMatrix);


	// -------------------------------------------------------------
	// --------Project 4--------------------------------------------
	// -------------------------------------------------------------
	m_Cloth->AddForce(glm::vec3(0.0f, -0.2f, 0.0f) * (float)dt);
	//m_Cloth->WindForce(glm::vec3(0.5f, 0, 0.2f) * (float)dt); // generate some wind each frame
	m_Cloth->Update(dt);
	m_Cloth->CheckSphereCollsion(ballPosition - clothPosition, ballRadius);

	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitClothShader)->Bind();
	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitClothShader)->UploadUniform1f("u_AmbientIntensity", 1.0f);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitClothShader)->UploadUniform3f("u_BaseColor", glm::vec3(1.0f));
	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitClothShader)->UploadUniform3f("u_LightColor", lightColor);
	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitClothShader)->UploadUniform3f("u_LightPosition", lightPosition);
	for (int i = 0; i < m_Cloth->GetParticleCount(); ++i)
	{
		glm::vec3 offsetPosition = m_Cloth->GetParticleOffsetPosition(i);
		std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitClothShader)->UploadUniform3f("u_OffsetPositions[" + std::to_string(i) + "]", offsetPosition);
	}
	std::dynamic_pointer_cast<Basement::OpenGLShader>(simpleLitClothShader)->Unbind();

	//m_Cloth->Draw();
	glm::mat4 clothTransform = glm::translate(glm::mat4(1.0f), clothPosition);
	m_Cloth->DrawParticle(simpleLitClothShader, clothTransform);

	//////--------------
	////// Draw Floor
	//////--------------
	//glm::mat4 floorModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, FloorPositionY, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(FloorSize, 1.0f, FloorSize));
	//m_FloorTexture->Bind();
	//Basement::Renderer::SubmitArrays(floorShader, m_FloorVAO, 0, 6, floorModel);


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


