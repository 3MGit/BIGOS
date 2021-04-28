#include "PBRDemoLayer.h"
#include <memory>

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
	BIGOS::Light u_Lights[20];
	uint32_t u_LightsCount;
};

__declspec(align(16))
struct POConstantBufferData
{
	BIGOS::math::mat4 u_Transform;
	BIGOS::math::mat4 u_ViewProj;
	BIGOS::math::mat4 u_InvModelViewProj;
};

PBRDemoLayer::PBRDemoLayer()
	: Layer("PBRDemoLayer")
{
}

void PBRDemoLayer::OnAttach()
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

	m_PBRShader = BIGOS::Shader::Create("assets/shaders/pbr.hlsl");
	m_ScreenShader = BIGOS::Shader::Create("assets/shaders/screen.hlsl");
	m_SkyboxShader = BIGOS::Shader::Create("assets/shaders/skybox.hlsl");

	m_NormalTexture = BIGOS::Texture2D::Create("assets/textures/Bricks053/Bricks053_1K_normal.png");
	m_WhiteTexture = BIGOS::Texture2D::Create("assets/textures/white.png");
	//m_WhiteTexture = BIGOS::Texture2D::Create("assets/models/Cerberus/Textures/Metalness.tga");
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
	m_SphereMesh = BIGOS::MeshGenerator::CreateSphere(1.0f, 64, 64);

	m_CBPerObject = BIGOS::ConstantBuffer::Create(sizeof(POConstantBufferData));
	m_CBPerFrame = BIGOS::ConstantBuffer::Create(sizeof(PFConstantBufferData));
	m_SkyboxCB = BIGOS::ConstantBuffer::Create(sizeof(SkyboxConstantBufferData));

	m_Material = new BIGOS::Material(m_PBRShader);

	m_StoneMaterial = new BIGOS::Material(m_PBRShader);
	m_StoneMaterial->SetAlbedoTexture(BIGOS::Texture2D::Create("assets/textures/Rocks022/Albedo.png"));
	m_StoneMaterial->SetRoughnessTexture(BIGOS::Texture2D::Create("assets/textures/Rocks022/Roughness.png"));
	m_StoneMaterial->SetNormalTexture(BIGOS::Texture2D::Create("assets/textures/Rocks022/Normal.png"));
	m_StoneMaterial->SetAOTexture(BIGOS::Texture2D::Create("assets/textures/Rocks022/AmbientOcclusion.png"));

	m_LightManager = BIGOS::LightManager::Create();

	m_Lights[0] = new BIGOS::Light({ -10.0f, 10.0f, 10.0f }, {300.0f, 300.0f, 300.0f, 300.0f});
	m_Lights[1] = new BIGOS::Light({ 10.0f, 10.0f, 10.0f }, { 300.0f, 300.0f, 300.0f, 300.0f });
	m_Lights[2] = new BIGOS::Light({ -10.0f, -10.0f, 10.0f }, { 300.0f, 300.0f, 300.0f, 300.0f });
	m_Lights[3] = new BIGOS::Light({ 10.0f, -10.0f, 10.0f }, { 300.0f, 300.0f, 300.0f, 300.0f });

	m_EditorCamera = BIGOS::EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);
}

void PBRDemoLayer::OnDetach()
{
	delete m_GridMesh;
	delete m_SphereMesh;
	delete m_Skybox;
	delete m_ScreenMesh;
	delete m_Lights[0];
	delete m_Lights[1];
	delete m_Lights[2];
	delete m_Lights[3];
	delete m_Material;
	delete m_StoneMaterial;
}

void PBRDemoLayer::OnUpdate(BIGOS::Utils::Timestep ts)
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
	m_EnvironmentMap->Unbind(0);


	// Scene update
	PFConstantBufferData cbPerFrame;
	cbPerFrame.u_CameraPosition = m_EditorCamera.GetPosition();
	cbPerFrame.u_Lights[0] = *m_Lights[0];
	cbPerFrame.u_Lights[1] = *m_Lights[1];
	cbPerFrame.u_Lights[2] = *m_Lights[2];
	cbPerFrame.u_Lights[3] = *m_Lights[3];
	cbPerFrame.u_LightsCount = m_LightManager->GetCount();
	m_CBPerFrame->SetData(&cbPerFrame, sizeof(cbPerFrame));
	m_CBPerFrame->Bind(0);

	POConstantBufferData cbPerObject;
	
	size_t rows = 7, cols = 7;
	size_t matIndex = 0;
	for (size_t i = 0; i < rows; i++)
	{
		m_Material->SetMetalic((float)i / (float)rows);
		for (size_t j = 0; j < cols; j++)
		{
			BIGOS::math::mat4 tempTrans = BIGOS::math::mat4::Translate({ -6.0f + 2 * j, 6.0f - 2 * i, -0.0f });
			BIGOS::math::mat4 tempRot = BIGOS::math::mat4::Rotate(0, { 0, 1, 0 });
			BIGOS::math::mat4 tempScale = BIGOS::math::mat4::Scale({ 0.7f, 0.7f, 0.7f });

			cbPerObject.u_Transform = tempTrans * tempRot * tempScale;
			cbPerObject.u_ViewProj = m_EditorCamera.GetViewProjection();
			cbPerObject.u_InvModelViewProj = BIGOS::math::mat4::Invert(cbPerObject.u_Transform * cbPerObject.u_ViewProj);
			m_CBPerObject->SetData(&cbPerObject, sizeof(cbPerObject));
			m_CBPerObject->Bind(1); // param is register

			m_Material->SetRoughness(BIGOS::math::clamp((float)j / (float)cols, 0.05f, 1.0f));
			m_Material->Bind();
			//m_StoneMaterial->Bind();
			
			m_SphereMesh->Render();
			matIndex++;
			
		}
	}

	m_Framebuffer->Unbind();

	// Rendering texture to screen
	m_ScreenShader->Bind();
	m_Framebuffer->BindTexture(0);
	//m_WhiteTexture->Bind(0);
	m_ScreenMesh->Render();
	//m_WhiteTexture->Unbind(0);
	m_Framebuffer->UnbindTexture(0);
	m_ScreenShader->Unbind();

	m_Rotation += ts / 50.0f;
}

void PBRDemoLayer::OnImGuiRender()
{
	//ImGui::DragFloat3("Wall position", m_WallPosition.ptr());
	//ImGui::Image(m_Framebuffer->GetTexture(), ImVec2(m_Framebuffer->GetSpecification().Width, m_Framebuffer->GetSpecification().Height));

	ImGui::Begin("Memory");
	ImGui::Text("Currentlly in use: %s", BIGOS::MemoryManager::Get()->BytesToString(BIGOS::MemoryManager::Get()->m_MemoryStats.currentUsed).c_str());
	ImGui::Text("Total allocated: %s", BIGOS::MemoryManager::Get()->BytesToString(BIGOS::MemoryManager::Get()->m_MemoryStats.totalAllocated).c_str());
	ImGui::Text("Total freed: %s", BIGOS::MemoryManager::Get()->BytesToString(BIGOS::MemoryManager::Get()->m_MemoryStats.totalFreed).c_str());
	ImGui::Text("Total allocations: %d", BIGOS::MemoryManager::Get()->m_MemoryStats.totalAllocations);
	ImGui::Text("");
	std::string availableMemory = BIGOS::MemoryManager::Get()->BytesToString(BIGOS::MemoryManager::Get()->GetSystemInfo().availablePhysicalMemory);
	ImGui::Text("System memory");
	float values[1] = { (float)BIGOS::MemoryManager::Get()->GetSystemInfo().availablePhysicalMemory / 1024 * 1024 * 1024 };
	ImVec2 size = { 75.0f, 100.0f };
	float maxMem = (float)BIGOS::MemoryManager::Get()->GetSystemInfo().totalPhysicalMemory / 1024 * 1024 * 1024;
	ImGui::Text("  %s", availableMemory.c_str());
	ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, maxMem, size);
	ImGui::Text("\tCPU");
	ImGui::End();

	//ImGui::Begin("Test");
	//ImVec2 size = { 40.0f, 100.0f };
	//const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
	//ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);
	//ImGui::End();

	//ImGui::ShowDemoWindow();
}

void PBRDemoLayer::OnEvent(BIGOS::Event& e)
{
	//BGS_TRACE(e.ToString());
	m_EditorCamera.OnEvent(e);

	BIGOS::EventManager manager(e);
	manager.Dispatch<BIGOS::KeyPressedEvent>(BGS_BIND_EVENT_FN(PBRDemoLayer::OnKeyPressed));
}

bool PBRDemoLayer::OnKeyPressed(BIGOS::KeyPressedEvent& e)
{
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::Escape))
		BIGOS::Application::Get().Close();

	return true;
}