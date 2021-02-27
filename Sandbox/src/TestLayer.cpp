#include "TestLayer.h"
#include <memory>

__declspec(align(16))
struct ConstantBufferData
{
	BIGOS::math::mat4 u_Transform;
	BIGOS::math::mat4 u_ViewProj;
	BIGOS::math::vec4 u_Color;
};

TestLayer::TestLayer()
	: Layer("TestLayer")
{
}

void TestLayer::OnAttach()
{
	m_Shader = BIGOS::Shader::Create("assets/shaders/color.hlsl");
	m_Shader->Bind();

	m_Cube = BIGOS::MeshGenerator::CreateCube(1);

	m_CBPerObject = BIGOS::ConstantBuffer::Create(sizeof(ConstantBufferData));

	m_EditorCamera = BIGOS::EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);
}

void TestLayer::OnDetach()
{
	delete m_Cube;
}

void TestLayer::OnUpdate(BIGOS::Utils::Timestep ts)
{
	// Resize
	uint32_t width = BIGOS::Application::Get().GetWindow()->GetWidth();
	uint32_t height = BIGOS::Application::Get().GetWindow()->GetHeight();
	m_EditorCamera.SetViewportSize(width, height);

	// Input
	// Editor cammera controlls movement
	m_EditorCamera.OnUpdate(ts);

#ifdef INPUT 
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::A))
		m_Position -= BIGOS::math::vec3(1.0f, 0.0f, 0.0f) * m_Speed;
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::W))
		m_Position += BIGOS::math::vec3(0.0f, 1.0f, 0.0f) * m_Speed;
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::D))
		m_Position += BIGOS::math::vec3(1.0f, 0.0f, 0.0f) * m_Speed;
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::S))
		m_Position -= BIGOS::math::vec3(0.0f, 1.0f, 0.0f) * m_Speed;
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::T))
		m_Position -= BIGOS::math::vec3(0.0f, 0.0f, 1.0f) * m_Speed;
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::G))
		m_Position += BIGOS::math::vec3(0.0f, 0.0f, 1.0f) * m_Speed;
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::E))
		m_Scale += BIGOS::math::vec3(1.0f, 1.0f, 1.0f) * m_Speed;
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::Q))
		m_Scale -= BIGOS::math::vec3(1.0f, 1.0f, 1.0f) * m_Speed;

#endif // 0
	// Scene update
	ConstantBufferData cbPerObject;
	BIGOS::math::mat4 tempTrans = BIGOS::math::mat4::Translate(m_Position);
	BIGOS::math::mat4 tempRot = BIGOS::math::mat4::Rotate(30.0f, { 1, 1, 0 });
	BIGOS::math::mat4 tempScale = BIGOS::math::mat4::Scale(m_Scale);

	cbPerObject.u_Transform = BIGOS::math::mat4::Identity();
	cbPerObject.u_Transform *= tempTrans * tempRot * tempScale;
	cbPerObject.u_ViewProj = m_EditorCamera.GetViewProjection();
	cbPerObject.u_Color = m_CubeColor;
	m_CBPerObject->SetData(&cbPerObject, sizeof(cbPerObject));
	m_CBPerObject->Bind(0); // first param is register

	// Rendering
	BIGOS::RenderCommand::SetClearColor(m_ClearColor);
	BIGOS::RenderCommand::Clear();

	m_Cube->Render();
}

void TestLayer::OnImGuiRender()
{
}

void TestLayer::OnEvent(BIGOS::Event& e)
{
	//BGS_TRACE(e.ToString());
	m_EditorCamera.OnEvent(e);

	BIGOS::EventManager manager(e);
	manager.Dispatch<BIGOS::KeyPressedEvent>(BGS_BIND_EVENT_FN(TestLayer::OnKeyPressed));
}

bool TestLayer::OnKeyPressed(BIGOS::KeyPressedEvent& e)
{
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::Escape))
		BIGOS::Application::Get().Close();
		
	return true;
}
