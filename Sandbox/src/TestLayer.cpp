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


	m_VertexBuffer = BIGOS::VertexBuffer::Create(list, sizeof(list));
	m_VertexBuffer->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "a_Position" }
		});
	m_VertexBuffer->Bind();
	
}

void TestLayer::OnDetach()
{
}

void TestLayer::OnUpdate(BIGOS::Utils::Timestep ts)
{
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::A))
		BGS_CORE_INFO("A");
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::W))
		BGS_CORE_INFO("W");
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::D))
		BGS_CORE_INFO("D");
	if (BIGOS::Input::IsKeyPressed(BIGOS::Key::S))
		BGS_CORE_INFO("S");
	// Rendering
	BIGOS::RenderCommand::SetClearColor(m_ClearColor);
	BIGOS::RenderCommand::Clear();
}

void TestLayer::OnImGuiRender()
{
}

void TestLayer::OnEvent(BIGOS::Event& e)
{
}
