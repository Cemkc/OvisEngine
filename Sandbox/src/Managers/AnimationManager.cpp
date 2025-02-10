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
		m_AnimationArray[i].TileToGet= nullptr;
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
			glm::vec3 positionToGet = animation.TileToGet->GetTransform().GetPosition();
			positionToGet.z = 0.1f;

			direction = glm::normalize(positionToGet - entityPos);

			animation.ObjectToMove->GetTransform().SetPosition(entityPos + direction * s_AnimationSpeed * (float)Time::DeltaTime());

			if (glm::distance(positionToGet, entityPos) < 0.02f)
			{
				animation.ObjectToMove->GetTransform().SetPosition(positionToGet);
				animation.TileToGet->SetTileObject(animation.ObjectToMove);
				animation.Moving = false;
				animation.ObjectToMove = nullptr;
				GridManager::Instance().RunningSequences--;
			}
		}
	}
}

void AnimationManager::MoveObject(std::shared_ptr<TileObject>& tileObject, Tile* tile)
{
	for (int i = 0; i < GridManager::TileCount(); i++)
	{
		if (m_AnimationArray[i].Moving == false)
		{
			m_AnimationArray[i].Moving = true;
			m_AnimationArray[i].ObjectToMove = tileObject;
			m_AnimationArray[i].TileToGet = tile;
			GridManager::Instance().RunningSequences++;
			break;
		}
	}
}
