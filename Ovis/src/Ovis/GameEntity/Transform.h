#pragma once

#include <glm/glm.hpp>

namespace Ovis
{
	class Transform
	{
	public:
		Transform();

		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

		const glm::mat4& GetTransformationMatrix() const { return m_TransformationMatrix; }
		
		glm::vec3& GetPosition() { return m_Position; }
		glm::vec3& GetRotation() { return m_Rotation; }
		glm::vec3& GetScale() { return m_Scale; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetRotation() const { return m_Rotation; }
		const glm::vec3& GetScale() const { return m_Scale; }

		void SetPosition(glm::vec3& position);

		void SetRotatin(glm::vec3& rotation);

		void SetScale(glm::vec3& scale);

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

		glm::mat4 m_TransformationMatrix;

		glm::mat4 CalculateTranformMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	};
}
