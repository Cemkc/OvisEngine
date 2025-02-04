#include "AnimationManager.h"

AnimationManager* AnimationManager::s_Instance = nullptr;

bool AnimationManager::s_MovingObject = false;
float AnimationManager::s_AnimationSpeed = 0.5f;

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
			direction = glm::normalize(animation.PositionToGet - entityPos);
			animation.ObjectToMove->GetTransform().SetPosition(entityPos + direction * s_AnimationSpeed * (float)Time::DeltaTime());

			if (glm::distance(animation.PositionToGet, entityPos) < 0.02f)
			{
				glm::vec3 tileObjPos = { animation.PositionToGet.x, animation.PositionToGet.y, 1.0f };
				animation.ObjectToMove->GetTransform().SetPosition(tileObjPos);
				animation.Moving = false;
			}
		}
	}
}

void AnimationManager::MoveObject(GameEntity& entity, glm::vec3 pos)
{
	for (int i = 0; i < 36; i++)
	{
		if (m_AnimationArray[i].Moving == false)
		{
			m_AnimationArray[i].Moving = true;
			m_AnimationArray[i].ObjectToMove = &entity;
			m_AnimationArray[i].PositionToGet = pos;
			break;
		}
	}
}
