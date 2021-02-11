#include "TestLayer.h"
#include <memory>

TestLayer::TestLayer()
	: Layer("TestLayer")
{
}

void TestLayer::OnAttach()
{
	float list[3*3] =
	{
		//X - Y - Z
		-0.5f,-0.5f,0.0f, // POS1
		 0.0f,0.5f,0.0f, // POS2
		 0.5f,-0.5f,0.0f
	};

	m_Shader = BIGOS::Shader::Create("assets/shaders/shader.shader");
	m_Shader->Bind();

	m_VertexBuffer = BIGOS::VertexBuffer::Create(list, sizeof(list));
	m_VertexBuffer->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "POSITION" }
		});
	m_VertexBuffer->Bind();
	
}

void TestLayer::OnDetach()
{
}

void TestLayer::OnUpdate(BIGOS::Utils::Timestep ts)
{
	
	// Rendering
	BIGOS::RenderCommand::SetClearColor(m_ClearColor);
	BIGOS::RenderCommand::Clear();

	BIGOS::RenderCommand::Draw(9, 0);

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
