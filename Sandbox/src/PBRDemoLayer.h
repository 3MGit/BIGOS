#pragma once

#include <BIGOS.h>

class PBRDemoLayer : public BIGOS::Layer
{
public:
	PBRDemoLayer();
	virtual ~PBRDemoLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(BIGOS::Utils::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(BIGOS::Event& e) override;

	bool OnKeyPressed(BIGOS::KeyPressedEvent& e);
private:
	BIGOS::math::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	BIGOS::math::vec3 m_WallPosition = { 0.0f, 0.0f, -4.0f };

	float m_Rotation = 0;

	std::shared_ptr<BIGOS::Shader> m_PBRShader;
	std::shared_ptr<BIGOS::Shader> m_ScreenShader;
	std::shared_ptr<BIGOS::Shader> m_SkyboxShader;

	std::shared_ptr<BIGOS::Framebuffer> m_Framebuffer;

	std::shared_ptr<BIGOS::Texture2D> m_BRDFTexture;
	std::shared_ptr<BIGOS::TextureCube> m_EnvironmentMap;
	std::shared_ptr<BIGOS::TextureCube> m_TestCube;

	std::shared_ptr<BIGOS::ConstantBuffer> m_CBPerObject;
	std::shared_ptr<BIGOS::ConstantBuffer> m_CBPerFrame;
	std::shared_ptr<BIGOS::ConstantBuffer> m_SkyboxCB;

	BIGOS::Mesh* m_SphereMesh = nullptr;
	BIGOS::Mesh* m_Skybox = nullptr;
	BIGOS::Mesh* m_GridMesh = nullptr;
	BIGOS::Mesh* m_ScreenMesh = nullptr;

	std::unique_ptr<BIGOS::LightManager> m_LightManager;
	
	BIGOS::Light* m_Lights[4];
	BIGOS::Material* m_Material;
	BIGOS::Material* m_StoneMaterial;
	BIGOS::Material* m_WornMetal;

	BIGOS::EditorCamera m_EditorCamera;
};
