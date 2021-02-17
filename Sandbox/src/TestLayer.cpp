#include "TestLayer.h"
#include <memory>

struct Vertex
{
	BIGOS::math::vec3 m_Position;
	BIGOS::math::vec4 color;
};

__declspec(align(16))
struct ConstantData
{
	BIGOS::math::mat4 u_World;
	BIGOS::math::mat4 u_View;
	BIGOS::math::mat4 u_Proj;
};

TestLayer::TestLayer()
	: Layer("TestLayer")
{
}

void TestLayer::OnAttach()
{
	Vertex cubeVertices[] =
	{
		{BIGOS::math::vec3(-0.5f,-0.5f,-0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
		{BIGOS::math::vec3( 0.5f,-0.5f,-0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
		{BIGOS::math::vec3( 0.5f, 0.5f,-0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
		{BIGOS::math::vec3(-0.5f, 0.5f,-0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
		{BIGOS::math::vec3(-0.5f,-0.5f, 0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
		{BIGOS::math::vec3( 0.5f,-0.5f, 0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
		{BIGOS::math::vec3( 0.5f, 0.5f, 0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
		{BIGOS::math::vec3(-0.5f, 0.5f, 0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	uint32_t cubeIndices[] =
	{
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};

	Vertex list[] =
	{
		// X    Y     Z
		{BIGOS::math::vec3(-0.5f,-0.5f, -0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },	// POS0 + COLOR0
		{BIGOS::math::vec3(-0.5f, 0.5f, -0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },	// POS1 + COLOR1
		{BIGOS::math::vec3( 0.5f, 0.5f, -0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },	// POS2 + COLOR2
		{BIGOS::math::vec3( 0.5f,-0.5f, -0.5f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) }		// POS3 + COLOR3
		//{BIGOS::math::vec3( 0.6f,-1.0f, 0.0f), BIGOS::math::vec4(1.0f, 1.0f, 0.0f, 1.0f) },	// POS4 + COLOR4
		//{BIGOS::math::vec3( 1.0f,-0.6f, 0.0f), BIGOS::math::vec4(1.0f, 1.0f, 1.0f, 1.0f) },	// POS5 + COLOR5
		//{BIGOS::math::vec3( 1.0f,-1.0f, 0.0f), BIGOS::math::vec4(1.0f, 0.0f, 1.0f, 1.0f) }	// POS6 + COLOR6
	};

	uint32_t indices[] =
	{
		0, 1, 2,			// 1st triangle
		0, 2, 3				// 2nd triangle
		//4, 5, 6			// 3rd triangle
	};

	m_Shader = BIGOS::Shader::Create("assets/shaders/color.hlsl");
	m_Shader->Bind();

	m_VertexBuffer = BIGOS::VertexBuffer::Create(sizeof(list));
	m_VertexBuffer->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "POSITION" },
			{ BIGOS::ShaderDataType::Float4, "COLOR"	}
		});
	m_VertexBuffer->SetData(cubeVertices, sizeof(cubeVertices));
	m_VertexBuffer->Bind();

	m_IndexBuffer = BIGOS::IndexBuffer::Create(cubeIndices, ARRAYSIZE(cubeIndices));
	m_IndexBuffer->Bind();	

	m_ConstantBuffer = BIGOS::ConstantBuffer::Create(sizeof(ConstantData));
	m_ConstantBuffer->Bind();
}

void TestLayer::OnDetach()
{

}

void TestLayer::OnUpdate(BIGOS::Utils::Timestep ts)
{
	// Input
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
		m_Scale += BIGOS::math::vec3(1.0f, 1.0f, 0.0f) * m_Speed;
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::Q))
		m_Scale -= BIGOS::math::vec3(1.0f, 1.0f, 0.0f) * m_Speed;


	// Scene update
	ConstantData cd;
	BIGOS::math::mat4 tempPos = BIGOS::math::mat4::Translate(m_Position);
	BIGOS::math::mat4 tempRot = BIGOS::math::mat4::Rotate(30.0f, { 1, 1, 0 });
	BIGOS::math::mat4 tempScale = BIGOS::math::mat4::Scale(m_Scale);
	cd.u_World = BIGOS::math::mat4::Identity();

	uint32_t width = BIGOS::Application::Get().GetWindow()->GetWidth();
	uint32_t height = BIGOS::Application::Get().GetWindow()->GetHeight();

	cd.u_World *= tempPos;
	cd.u_World *= tempRot;
	cd.u_World *= tempScale;
	cd.u_View = BIGOS::math::mat4::Identity();
	//cd.u_Proj = BIGOS::math::mat4::Orthographic(-1.0f * width / 400.0f, 1.0f * width / 400.0f, -1.0f * height/400.0f, 1.0f * height / 400.0f, -1.0f, 1.0f);
	cd.u_Proj = BIGOS::math::mat4::Perspective(65.0f, (float)width / (float)height, 0.1f, 10.0f);
	m_ConstantBuffer->SetData(&cd, sizeof(cd));

	// Rendering
	BIGOS::RenderCommand::SetClearColor(m_ClearColor);
	BIGOS::RenderCommand::Clear();

	BIGOS::RenderCommand::DrawIndexed(m_IndexBuffer->GetCount());
}

void TestLayer::OnImGuiRender()
{
}

void TestLayer::OnEvent(BIGOS::Event& e)
{
	BIGOS::EventManager manager(e);
	manager.Dispatch<BIGOS::KeyPressedEvent>(BGS_BIND_EVENT_FN(TestLayer::OnKeyPressed));
}

bool TestLayer::OnKeyPressed(BIGOS::KeyPressedEvent& e)
{
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::Escape))
		BIGOS::Application::Get().Close();
		
	return true;
}
