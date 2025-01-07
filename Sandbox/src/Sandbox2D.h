#pragma once

#include "Ovis.h"

class Sandbox2D : public Ovis::Layer
{
private:
	// std::shared_ptr<Ovis::Renderer2D> m_Renderer2D;
	std::shared_ptr<Ovis::OrthographicCameraController> m_CameraController;

	std::shared_ptr<Ovis::VertexArray> m_SquareVertexArray;
	Ovis::ShaderLibrary m_ShaderLibrary;

	std::shared_ptr<Ovis::Texture2D> m_CheckerBoardTexture;
	std::shared_ptr<Ovis::Texture2D> m_RocketTexture;
	std::shared_ptr<Ovis::Texture2D> m_FrogTexture;

	float m_SquareWidht = 0.5f;
	glm::vec4 m_QuadColor1 = glm::vec4(0.6f, 0.1f, 0.8f, 1.0f);
	glm::vec4 m_QuadColor2 = glm::vec4(0.6f, 0.1f, 0.8f, 1.0f);
public:
	Sandbox2D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Ovis::Event& event) override;
};