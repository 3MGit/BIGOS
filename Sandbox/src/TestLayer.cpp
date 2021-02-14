#include "TestLayer.h"
#include <memory>

TestLayer::TestLayer()
	: Layer("TestLayer")
{
}

void TestLayer::OnAttach()
{
	float list[] =
	{
		// X    Y     Z
		-0.5f,-0.5f, 0.0f,	// POS0
		-0.5f, 0.5f, 0.0f,	// POS1
		 0.5f, 0.5f, 0.0f,	// POS2
		 0.5f,-0.5f, 0.0f,	// POS3
		 0.8f,-1.0f, 0.0f,	// POS4
		 1.0f,-0.8f, 0.0f,	// POS5
		 1.0f,-1.0f, 0.0f	// POS6
	};

	uint32_t indices[] =
	{
		0, 1, 2,			// 1st triangle
		0, 2, 3,			// 2nd triangle
		4, 5, 6				// 3rd triangle
	};

	m_Shader = BIGOS::Shader::Create("assets/shaders/color.hlsl");
	m_Shader->Bind();

	m_VertexBuffer = BIGOS::VertexBuffer::Create(list, sizeof(list));
	m_VertexBuffer->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "POSITION" }
		});
	m_VertexBuffer->Bind();

	m_IndexBuffer = BIGOS::IndexBuffer::Create(indices, ARRAYSIZE(indices));
	m_IndexBuffer->Bind();	
}

void TestLayer::OnDetach()
{
}

void TestLayer::OnUpdate(BIGOS::Utils::Timestep ts)
{
	
	// Rendering
	BIGOS::RenderCommand::SetClearColor(m_ClearColor);
	BIGOS::RenderCommand::Clear();

	BIGOS::RenderCommand::DrawIndexed(m_IndexBuffer->GetCount());

	// Input test
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::A))
		BGS_INFO("A");
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::W))
		BGS_INFO("W");
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::D))
		BGS_INFO("D");
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::S))
		BGS_INFO("S");

	// Tracing stats
	//BGS_TRACE("%d", BIGOS::Application::Get().GetFPS());
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
	/*
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::A))
		BGS_INFO("A");
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::W))
		BGS_INFO("W");
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::D))
		BGS_INFO("D");
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::S))
		BGS_INFO("S");
		*/
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::Escape))
		BIGOS::Application::Get().Close();
		
	BGS_INFO(e.ToString());
	return true;
}
