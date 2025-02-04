#include "ovpch.h"

#include "GameEntity.h";
#include "Ovis/Utils/MatrixUtils.h"
#include "EntityManager.h"

namespace Ovis
{
	void GameEntity::Init()
	{
		m_Parent = nullptr;
		m_Id = IDGenerator::generateID();
		EntityManager::OnEntityCreate(this,  m_Id);
	}

	GameEntity::GameEntity()
		:m_Transform(Transform())
	{ 
		Init();
	}

	GameEntity::GameEntity(std::string& name)
		:m_Transform(Transform()), m_Name(name)
	{
		Init();
	}

	GameEntity::~GameEntity()
	{
		EntityManager::OnEntityDestroy(m_Id);
	}

	// This function needs to return by value since the result of multiplication cannot outlive the scope of the function
	const glm::mat4 GameEntity::GetTransformationMatrix() const
	{
		/*glm::mat4 result = glm::mat4(1.0f);

		if (m_Parent != nullptr)
			result = m_Transform.GetTransformationMatrix() * m_Parent->GetTransformationMatrix();
		else
			result = m_Transform.GetTransformationMatrix();

		return result;*/

		if (m_Parent != nullptr)
			return m_Parent->GetTransform().GetTransformationMatrix() * m_Transform.GetTransformationMatrix();
		else
			return m_Transform.GetTransformationMatrix();
	}

	void GameEntity::AssignParent(GameEntity& parent)
	{
		m_Parent = &parent;
	}

	void GameEntity::AssignChild(GameEntity& child)
	{
		OV_CORE_ASSERT(m_Children.find(child.GetId()) == m_Children.end(), "Children map contains child")

		m_Children[child.GetId()] = &child;
	}

	void GameEntity::RemoveChild(GameEntity& child)
	{
	}
}