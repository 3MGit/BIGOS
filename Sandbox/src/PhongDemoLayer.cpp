#include "PhongDemoLayer.h"
#include <memory>
#include "DemoMaterials.h"

#include <imgui/imgui.h>

__declspec(align(16))
struct SkyboxConstantBufferData
{
	BIGOS::math::mat4 u_ModelViewProj;
};

__declspec(align(16))
struct PFConstantBufferData
{
	BIGOS::math::vec3 u_CameraPosition;
	BIGOS::PhongLight u_Light[4];
};

__declspec(align(16))
struct POConstantBufferData
{
	BIGOS::math::mat4 u_Transform;
	BIGOS::math::mat4 u_ViewProj;
	BIGOS::math::mat4 u_InvModelViewProj;
	BIGOS::PhongMaterial u_Material;
};

PhongDemoLayer::PhongDemoLayer()
	: Layer("PhongDemoLayer")
{
}

void PhongDemoLayer::OnAttach()
{
	std::string environmentFiles[6] =
	{
		"assets/textures/skybox/lycksele/posx.png",
		"assets/textures/skybox/lycksele/negx.png",
		"assets/textures/skybox/lycksele/posy.png",
		"assets/textures/skybox/lycksele/negy.png",
		"assets/textures/skybox/lycksele/posz.png",
		"assets/textures/skybox/lycksele/negz.png"
	};

	m_Shader = BIGOS::Shader::Create("assets/shaders/phong.hlsl");
	m_Shader->Bind();
	m_ScreenShader = BIGOS::Shader::Create("assets/shaders/screen.hlsl");
	m_PBRShader = BIGOS::Shader::Create("assets/shaders/pbr.hlsl");

	m_SkyboxShader = BIGOS::Shader::Create("assets/shaders/skybox.hlsl");

	m_Texture = BIGOS::Texture2D::Create("assets/textures/Bricks053/Bricks053_1K_color.png");
	m_NormalTexture = BIGOS::Texture2D::Create("assets/textures/Bricks053/Bricks053_1K_normal.png");
	m_WhiteTexture = BIGOS::Texture2D::Create("assets/textures/white.png");

	m_EnvironmentMap = BIGOS::TextureCube::Create(environmentFiles);

	BIGOS::FramebufferSpecification fbSpec;
	fbSpec.Attachments = { BIGOS::FramebufferTextureFormat::RGBA8 };
	fbSpec.Width = BIGOS::Application::Get().GetWindow()->GetWidth();
	fbSpec.Height = BIGOS::Application::Get().GetWindow()->GetHeight();
	m_Framebuffer = BIGOS::Framebuffer::Create(fbSpec);

	m_GridMesh = BIGOS::MeshGenerator::CreateGrid(3.0f, 3.0f, 2, 2);
	//m_CubeMesh = BIGOS::MeshGenerator::CreateBox({ 1.0f, 1.0f, 1.0f });
	m_Skybox = BIGOS::MeshGenerator::CreateBox({ 30.0f, 30.0f, 30.0f });
	m_ScreenMesh = BIGOS::MeshGenerator::CreateScreen();
	m_CubeMesh = BIGOS::MeshGenerator::CreateSphere(1.0f, 64, 64);
	
	m_CBPerObject = BIGOS::ConstantBuffer::Create(sizeof(POConstantBufferData));
	m_CBPerFrame = BIGOS::ConstantBuffer::Create(sizeof(PFConstantBufferData));
	m_SkyboxCB = BIGOS::ConstantBuffer::Create(sizeof(SkyboxConstantBufferData));

	m_EditorCamera = BIGOS::EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);

	m_Light[0] = new BIGOS::PhongLight(BIGOS::math::vec4(0.2f), BIGOS::math::vec4(0.5f), BIGOS::math::vec4(1.0f), BIGOS::math::vec3(-0.3f, 0.2f, -1.0f));

	m_Materials = materialPallete;
	m_BrickMaterial = new BIGOS::PhongMaterial(BIGOS::math::vec4(0.6f), BIGOS::math::vec4(0.5f), BIGOS::math::vec4(0.2f));
}

void PhongDemoLayer::OnDetach()
{
	delete m_GridMesh;
	delete m_CubeMesh;
	delete m_Skybox;
	delete m_ScreenMesh;
	delete m_BrickMaterial;
	delete m_Light[0];
}

void PhongDemoLayer::OnUpdate(BIGOS::Utils::Timestep ts)
{
	// Resize
	uint32_t width = BIGOS::Application::Get().GetWindow()->GetWidth();
	uint32_t height = BIGOS::Application::Get().GetWindow()->GetHeight();
	m_EditorCamera.SetViewportSize(width, height);

	//Clearing 
	BIGOS::RenderCommand::SetClearColor(m_ClearColor);
	BIGOS::RenderCommand::Clear();

	// Input
	// Editor cammera controlls movement
	m_EditorCamera.OnUpdate(ts);

	//Framebuffer
	m_Framebuffer->Bind();
	m_Framebuffer->ClearAttachment(0, m_ClearColor);

	//Envmap
	m_SkyboxShader->Bind();
	m_EnvironmentMap->Bind(0);
	SkyboxConstantBufferData skyboxCB;
	BIGOS::math::mat4 tempSkyboxTransform = BIGOS::math::mat4::Translate({ 0.0f, 0.0f, 0.0f });
	skyboxCB.u_ModelViewProj = BIGOS::math::mat4::Invert(m_EditorCamera.GetViewProjection());
	m_SkyboxCB->SetData(&skyboxCB, sizeof(skyboxCB));
	m_SkyboxCB->Bind(0);
	m_Skybox->Render();


	// Scene update
	m_Shader->Bind();
	PFConstantBufferData cbPerFrame;
	cbPerFrame.u_CameraPosition = m_EditorCamera.GetPosition();
	cbPerFrame.u_Light[0] = *m_Light[0];
	m_CBPerFrame->SetData(&cbPerFrame, sizeof(cbPerFrame));
	m_CBPerFrame->Bind(0);

	POConstantBufferData cbPerObject;
	 /*
	BIGOS::math::mat4 tempTrans = BIGOS::math::mat4::Translate(m_WallPosition);
	BIGOS::math::mat4 tempRot = BIGOS::math::mat4::Rotate(90.0f, { 1, 0, 0 });
	BIGOS::math::mat4 tempScale = BIGOS::math::mat4::Scale({ 5.0, 5.0f, 5.0f });

	cbPerObject.u_Transform = tempTrans * tempRot * tempScale;
	cbPerObject.u_ViewProj = m_EditorCamera.GetViewProjection();
	cbPerObject.u_InvModelViewProj = BIGOS::math::mat4::Invert(cbPerObject.u_Transform * cbPerObject.u_ViewProj);
	cbPerObject.u_Material = m_Materials[7];
	m_CBPerObject->SetData(&cbPerObject, sizeof(cbPerObject));
	m_CBPerObject->Bind(1); // param is register
	m_Texture->Bind();
	m_NormalTexture->Bind(1);
	m_GridMesh->Render();
	m_NormalTexture->Unbind(1);
	m_Texture->Unbind();
	*/
	
	m_WhiteTexture->Bind();

	size_t matIndex = 0;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			BIGOS::math::mat4 tempTrans = BIGOS::math::mat4::Translate({ -3.0f + 2 * j, 3.0f - 2 * i, -0.0f });
			BIGOS::math::mat4 tempRot = BIGOS::math::mat4::Rotate(0, { 0, 1, 0 });
			BIGOS::math::mat4 tempScale = BIGOS::math::mat4::Scale({ 0.7f, 0.7f, 0.7f });

			cbPerObject.u_Transform = tempTrans * tempRot * tempScale;
			cbPerObject.u_ViewProj = m_EditorCamera.GetViewProjection();
			cbPerObject.u_InvModelViewProj = BIGOS::math::mat4::Invert(cbPerObject.u_Transform * cbPerObject.u_ViewProj);
			cbPerObject.u_Material = m_Materials[matIndex];
			m_CBPerObject->SetData(&cbPerObject, sizeof(cbPerObject));
			m_CBPerObject->Bind(1); // param is register

			m_CubeMesh->Render();
			matIndex++;
		}
	}
	m_WhiteTexture->Unbind();
	
	m_Framebuffer->Unbind();
	m_Shader->Unbind();

	m_ScreenShader->Bind();
	m_Framebuffer->BindTexture(0);
	m_ScreenMesh->Render();
	m_Framebuffer->UnbindTexture(0);
	m_ScreenShader->Unbind();

	m_Rotation += ts/50.0f;
}

void PhongDemoLayer::OnImGuiRender()
{
	/*
	ImGui::Begin("Test");
	ImGui::Text("Hello World");
	ImGui::DragFloat3("Wall position", m_WallPosition.ptr());
	//ImGui::Image(m_Framebuffer->GetTexture(), ImVec2(m_Framebuffer->GetSpecification().Width, m_Framebuffer->GetSpecification().Height));
	ImGui::End();
	*/
}

void PhongDemoLayer::OnEvent(BIGOS::Event& e)
{
	//BGS_TRACE(e.ToString());
	m_EditorCamera.OnEvent(e);

	BIGOS::EventManager manager(e);
	manager.Dispatch<BIGOS::KeyPressedEvent>(BGS_BIND_EVENT_FN(PhongDemoLayer::OnKeyPressed));
}

bool PhongDemoLayer::OnKeyPressed(BIGOS::KeyPressedEvent& e)
{
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::Escape))
		BIGOS::Application::Get().Close();
		
	return true;
}
