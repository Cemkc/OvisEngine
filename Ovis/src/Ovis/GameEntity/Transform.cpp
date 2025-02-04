#include "ovpch.h"
#include "Transform.h"
#include "Ovis/Utils/MatrixUtils.cpp"

#include <glm/ext/matrix_transform.hpp>

namespace Ovis
{
	Transform::Transform()
		:m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f))
	{
		m_TransformationMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		m_TransformationMatrix = glm::rotate(m_TransformationMatrix, m_Rotation.z, { 0.0f, 0.0f, 1.0f });
		m_TransformationMatrix = glm::scale(m_TransformationMatrix, m_Scale);
	}

	Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		:m_Position(position), m_Rotation(rotation), m_Scale(scale)
	{
		m_TransformationMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		m_TransformationMatrix = glm::rotate(m_TransformationMatrix, m_Rotation.z, { 0.0f, 0.0f, 1.0f });
		m_TransformationMatrix = glm::scale(m_TransformationMatrix, m_Scale);
	}

	void Transform::SetPosition(glm::vec3& position)
	{
		m_TransformationMatrix[3][0] = position.x;
		m_TransformationMatrix[3][1] = position.y;
		m_TransformationMatrix[3][2] = position.z;

		// OV_CORE_INFO("Matrix:\n{0}", Utils::Mat4ToString(m_TransformationMatrix));

		m_Position = position;
	}

	void Transform::SetRotatin(glm::vec3& rotation)
	{
		m_Rotation = rotation;
		m_TransformationMatrix = CalculateTranformMatrix(m_Position, m_Rotation, m_Scale);

		// OV_CORE_INFO("Matrix:\n{0}", Utils::Mat4ToString(m_TransformationMatrix));
	}

	void Transform::SetScale(glm::vec3& scale)
	{
		m_Scale = scale;
		m_TransformationMatrix = CalculateTranformMatrix(m_Position, m_Rotation, m_Scale);

		// OV_CORE_INFO("Matrix:\n{0}", Utils::Mat4ToString(m_TransformationMatrix));
	}

	glm::mat4 Transform::CalculateTranformMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		glm::mat4 trans;

		trans = glm::translate(glm::mat4(1.0f), m_Position);
		trans = glm::rotate(trans, m_Rotation.z, { 0.0f, 0.0f, 1.0f });
		trans = glm::scale(trans, m_Scale);

		return trans;
	}

}