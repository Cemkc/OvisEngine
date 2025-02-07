#include "ovpch.h" 

#include <random>

#include "RocketTileObject.h"
#include "IHitableTileObject.h"
#include "Managers/AnimationManager.h"

RocketTileObject::RocketTileObject()
{
	// Create a random number generator
	std::random_device rd; // Obtain a random number from the system
	std::mt19937 gen(rd()); // Use Mersenne Twister engine
	std::uniform_int_distribution<int> dis(0, 1); // Distribution for the enum range
	
	m_Vertical = dis(gen);

	m_Name = "Rocket Tile";
	m_Type = TileObjectType::Rocket;
	m_Category = (TileObjectCategory)(m_Category | TileObjectCategory::HitableTileObject);
	m_Color = m_Vertical ? glm::vec4(1.0f, 0.0f, 0.863f, 1.0f) : glm::vec4(0.694f, 0.835f, 0.851f, 1.0f);
}

bool RocketTileObject::OnClick()
{
	OV_INFO("Rocket Clicked!");
	m_TileAPos = GridManager::Instance().TileIdToPos(m_Tile->GetTileId());
	m_TileBPos = GridManager::Instance().TileIdToPos(m_Tile->GetTileId());

	m_Active = true;
	GridManager::Instance().m_RunningSequences++;
	return true;
}

void RocketTileObject::OnUpdate()
{
	if (!m_Active) return;

	m_RockeFireTimer += Time::DeltaTime();

	if (m_RockeFireTimer < 0.2f) return;

	if (m_Vertical)
	{
		m_TileAPos.y += 1;
		m_TileBPos.y -= 1;
	}
	else
	{
		m_TileAPos.x += 1;
		m_TileBPos.x -= 1;
	}

	Tile* tileA = GridManager::Instance().GetTile(m_TileAPos);
	if (tileA != nullptr && tileA->GetTileObject()->GetTileObjectCategory() & TileObjectCategory::HitableTileObject)
	{
		IHitableTileObject* hitableTileObject = dynamic_cast<IHitableTileObject*>(tileA->GetTileObject().get());
		hitableTileObject->OnHit(1);
	}

	Tile* tileB = GridManager::Instance().GetTile(m_TileBPos);
	if (tileB != nullptr && tileB->GetTileObject()->GetTileObjectCategory() & TileObjectCategory::HitableTileObject)
	{
		IHitableTileObject* hitableTileObject = dynamic_cast<IHitableTileObject*>(tileB->GetTileObject().get());
		hitableTileObject->OnHit(1);
	}

	if (tileA == nullptr && tileB == nullptr)
	{
		m_Active = false;
		GridManager::Instance().m_RunningSequences--;
		GridManager::Instance().OnTileDestroy(m_Tile);
	}

	m_RockeFireTimer = 0.0f;

}

void RocketTileObject::OnHit(int damage)
{
	OnClick();
}
