#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Ovis
{
	enum class Projection
	{
		Perspective,
		Orthographic
	};

	class Camera
	{
	protected:
		static Camera* s_ActiveCamera;

		Projection m_Projection;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::vec3 m_Position;

		float m_Fov, m_NearPlane, m_FarPlane;
		int m_Widht, m_Height;
	public:
		Camera(float nearPlane, float farPlane);

		inline void SetPosition(glm::vec3 position) { m_Position = position; UpdateViewMatrix(); }
		inline glm::vec3 GetPosition() { return m_Position; }

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		const Projection GetProjection() const { return m_Projection; }

		virtual void UpdateViewMatrix() = 0;

		void SetActiveCamera(Camera* camera) { 
			s_ActiveCamera = camera; 
		}

		static glm::vec2 ScreenToWorldPoint(const float screenX, const float screenY);
	};

	class OrthographicCamera : public Camera
	{
	private:
		float m_Rotation;
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);
		void SetProjectionMatrix(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);

		inline void SetRotation(float rotation) { m_Rotation = rotation; UpdateViewMatrix(); }
	private:
		virtual void UpdateViewMatrix() override;
	};

	class PerspectiveCamera : public Camera
	{
	private:
		float m_Fov, m_AspectRatio;
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearPlane = 1.0f, float farPlane = 100.0f);
	private:
		virtual void UpdateViewMatrix() override;
	};

}