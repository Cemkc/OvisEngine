#include "AnimationManager.h"

AnimationManager* AnimationManager::s_Instance = nullptr;

bool AnimationManager::s_MovingObject = false;
float AnimationManager::s_AnimationSpeed = 1.0f;

AnimationManager::AnimationManager()
	:Layer("AnimationManager")
{
	OV_CORE_ASSERT(!s_Instance, "AnimationManager already exists!");
	s_Instance = this;

	for (int i = 0; i < m_AnimationArray.size(); i++)
	{
		m_AnimationArray[i].Moving = false;
		m_AnimationArray[i].ObjectToMove = nullptr;
		m_AnimationArray[i].PositionToGet = glm::vec3(0.0f);
	}
}

void AnimationManager::OnUpdate()
{
	glm::vec3 direction;
	for (auto& animation : m_AnimationArray)
	{
		if (animation.Moving == true)
		{
			glm::vec3 entityPos = animation.ObjectToMove->GetTransform().GetPosition();
			glm::vec3 positionToGet = glm::vec3(animation.PositionToGet.x, animation.PositionToGet.y, 0.1f);
			direction = glm::normalize(positionToGet - entityPos);
			OV_INFO("Direction magnitute: {0}", glm::length(direction));
			animation.ObjectToMove->GetTransform().SetPosition(entityPos + direction * s_AnimationSpeed * (float)Time::DeltaTime());

			if (glm::distance(positionToGet, entityPos) < 0.02f)
			{
				animation.ObjectToMove->GetTransform().SetPosition(positionToGet);
				animation.Moving = false;
				GridManager::Instance().m_RunningSequences--;
			}
		}
	}
}

void AnimationManager::MoveObject(GameEntity& entity, glm::vec3 pos)
{
	for (int i = 0; i < GridManager::TileNumber(); i++)
	{
		if (m_AnimationArray[i].Moving == false)
		{
			m_AnimationArray[i].Moving = true;
			m_AnimationArray[i].ObjectToMove = &entity;
			m_AnimationArray[i].PositionToGet = pos;
			GridManager::Instance().m_RunningSequences++;
			break;
		}
	}
}
