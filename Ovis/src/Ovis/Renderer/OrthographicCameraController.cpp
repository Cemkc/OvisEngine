#include "ovpch.h"
#include "OrthographicCameraController.h"

#include "Ovis/Core/Application.h"
#include "Ovis/Core/Input.h"
#include "Ovis/Core/KeyCodes.h"

#include <glm/glm.hpp>

namespace Ovis
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio), m_OrthoCam(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate()
	{
		m_CameraPosition = m_OrthoCam.GetPosition();

		if (Input::IsKeyPressed(OV_KEY_W))
		{
			m_CameraPosition.y += 1.0f * m_CameraMoveSpeed * (float)Time::DeltaTime();
		}
		if (Input::IsKeyPressed(OV_KEY_S))
		{
			m_CameraPosition.y -= 1.0f * m_CameraMoveSpeed * (float)Time::DeltaTime();
		}
		if (Input::IsKeyPressed(OV_KEY_A))
		{
			m_CameraPosition.x -= 1.0f * m_CameraMoveSpeed * (float)Time::DeltaTime();
		}
		if (Input::IsKeyPressed(OV_KEY_D))
		{
			m_CameraPosition.x += 1.0f * m_CameraMoveSpeed * (float)Time::DeltaTime();
		}

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(OV_KEY_E))
			{
				m_CameraRotation -= m_CameraRotationSpeed * (float)Time::DeltaTime();
			}
			if (Input::IsKeyPressed(OV_KEY_Q))
			{
				m_CameraRotation += m_CameraRotationSpeed * (float)Time::DeltaTime();
			}

			m_OrthoCam.SetRotation(m_CameraRotation);
		}

		m_OrthoCam.SetPosition(m_CameraPosition);

		m_CameraMoveSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&OrthographicCameraController::OnWindowResize, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::OnMouseScroll, this, std::placeholders::_1));
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_OrthoCam.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnMouseScroll(MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * 0.25f;
		m_ZoomLevel = glm::clamp(m_ZoomLevel, 0.0f, 10.f);
		m_OrthoCam.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}