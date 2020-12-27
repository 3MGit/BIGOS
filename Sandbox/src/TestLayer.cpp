#include "TestLayer.h"

TestLayer::TestLayer()
	: Layer("TestLayer")
{
}

void TestLayer::OnAttach()
{
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
}

void TestLayer::OnImGuiRender()
{
}

void TestLayer::OnEvent(BIGOS::Event& e)
{
}
