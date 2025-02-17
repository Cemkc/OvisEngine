#include "ovpch.h"

#include "Camera.h"
#include "Ovis/Core/Log.h"
#include "Ovis/Core/Window.h"
#include "Ovis/Core/Application.h"

namespace Ovis
{
	Camera* Camera::s_ActiveCamera = nullptr;

	Camera::Camera(float nearPlane, float farPlane)
		: m_Position(glm::vec3(0.0f)), m_Projection(Projection::Perspective),
		  m_ViewMatrix(1.0f), m_NearPlane(nearPlane), m_FarPlane(farPlane)
	{
	}

	glm::vec2 Camera::ScreenToWorldPoint(const float screenX, const float screenY)
	{
		float ndcX = (2 * screenX) / Application::Get().GetWindow().GetWidth() - 1; // Normalized device coordinates x (-1, 1 - Left To Right)
		float ndcY = 1 -(2 * screenY) / Application::Get().GetWindow().GetHeight(); // Normalized device coordinates x (-1, 1 - Bottom to Top)

		glm::vec4 worldPos = glm::inverse(s_ActiveCamera->m_ViewMatrix) * glm::inverse(s_ActiveCamera->m_ProjectionMatrix) * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);

		return { worldPos.x, worldPos.y };
	}

	/////////////////////////////////////
	// Orthographic Camera //////////////
	/////////////////////////////////////

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
		: Camera(nearPlane, farPlane), m_Rotation(0.0f)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
		SetActiveCamera(this);
	}

	void OrthographicCamera::SetProjectionMatrix(float left, float right, float bottom, float top, float nearPlane, float farPlane)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
	}

	void OrthographicCamera::UpdateViewMatrix()
	{
		OV_RENDER_PROFILE_FUNC();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
	}

	/////////////////////////////////////
	// Perspective Camera ///////////////
	/////////////////////////////////////

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
		: Camera(nearPlane, farPlane), m_Fov(fov), m_AspectRatio(aspectRatio)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearPlane, m_FarPlane);
		SetActiveCamera(this);
	}
	void PerspectiveCamera::UpdateViewMatrix()
	{
		OV_RENDER_PROFILE_FUNC();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		m_ViewMatrix = glm::inverse(transform);
	}
}
