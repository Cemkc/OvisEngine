#include "ovpch.h"

#include "Camera.h"
#include "Ovis/Core/Log.h"

namespace Ovis
{
	Camera::Camera(float nearPlane, float farPlane)
		: m_Position(glm::vec3(0.0f)), m_Projection(Projection::Perspective),
		  m_ViewMatrix(1.0f), m_NearPlane(nearPlane), m_FarPlane(farPlane)
	{
	}

	/////////////////////////////////////
	// Orthographic Camera //////////////
	/////////////////////////////////////

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
		: Camera(nearPlane, farPlane), m_Rotation(0.0f)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
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
	}
	void PerspectiveCamera::UpdateViewMatrix()
	{
		OV_RENDER_PROFILE_FUNC();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		m_ViewMatrix = glm::inverse(transform);
	}
}
