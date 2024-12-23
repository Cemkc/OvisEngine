#pragma once

#include "Ovis/Renderer/Camera.h"
#include "Ovis/Core/Window.h"

#include "Ovis/Events/ApplicationEvent.h"
#include "Ovis/Events/MouseEvent.h"

namespace Ovis
{
	class OrthographicCameraController
	{
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_OrthoCam;

		float m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraMoveSpeed = 1.0f, m_CameraRotationSpeed = 1.0f;
	public:
		OrthographicCameraController(float aspectRatio, bool rotation);

		void OnUpdate();
		void OnEvent(Event& event);

		OrthographicCamera GetCamera() { return m_OrthoCam; }
		const OrthographicCamera GetCamera() const { return m_OrthoCam; }

	private:
		bool OnWindowResize(WindowResizeEvent& event);
		bool OnMouseScroll(MouseScrolledEvent& event);
	};
}