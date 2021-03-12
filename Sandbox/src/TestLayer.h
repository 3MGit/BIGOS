#pragma once

#include <BIGOS.h>

class TestLayer : public BIGOS::Layer
{
public:
	TestLayer();
	virtual ~TestLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(BIGOS::Utils::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(BIGOS::Event& e) override;

	bool OnKeyPressed(BIGOS::KeyPressedEvent& e);
private:
	BIGOS::math::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	BIGOS::math::vec4 m_CubeColor = { 0.9f, 0.1f, 0.2f, 1.0f };
	BIGOS::math::vec3 m_Position = { 0.0f, 0.0f, -2.0f };
	BIGOS::math::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
	float m_Speed = 0.01f;

	std::shared_ptr<BIGOS::Shader> m_Shader;
	std::shared_ptr<BIGOS::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<BIGOS::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<BIGOS::ConstantBuffer> m_CBPerObject;

	BIGOS::Mesh* m_Cube = nullptr;
	BIGOS::Light* m_Light = nullptr;
	BIGOS::Material* m_Material = nullptr;

	BIGOS::EditorCamera m_EditorCamera;
};