#include "ovpch.h" 

#include <random>

#include "Harpoon.h"
#include "IHitableTileObject.h"
#include "Managers/AnimationManager.h"

std::shared_ptr<Texture2D> Harpoon::s_HarpoonVTexture = nullptr;
std::shared_ptr<Texture2D> Harpoon::s_HarpoonHTexture = nullptr;

Harpoon::Harpoon()
{
	// Create a random number generator
	std::random_device rd; // Obtain a random number from the system
	std::mt19937 gen(rd()); // Use Mersenne Twister engine
	std::uniform_int_distribution<int> dis(0, 1); // Distribution for the enum range
	
	m_Vertical = dis(gen);

	m_Name = "Harpoon";
	m_Type = TileObjectType::Harpoon;
	m_Category = (TileObjectCategory)(m_Category | TileObjectCategory::HitableTileObject);

	if (!s_HarpoonHTexture)
		s_HarpoonHTexture = Texture2D::Create("assets/textures/HarpoonH.png");

	if (!s_HarpoonVTexture)
		s_HarpoonVTexture = Texture2D::Create("assets/textures/HarpoonV.png");

	m_Color = m_Vertical ? glm::vec4(1.0f, 0.0f, 0.863f, 1.0f) : glm::vec4(0.694f, 0.835f, 0.851f, 1.0f);
}

bool Harpoon::OnClick()
{
	OV_INFO("Rocket Clicked!");
	if (m_Fired) return true;
	m_TileAPos = GridManager::Instance().TileIdToPos(m_Tile->GetTileId());
	m_TileBPos = GridManager::Instance().TileIdToPos(m_Tile->GetTileId());

	m_Fired = true;
	m_Active = true;
	GridManager::Instance().RunningSequences++;
	return true;
}

void Harpoon::OnUpdate()
{
	if (!m_Active) return;

	m_RockeFireTimer += Time::DeltaTime();

	if (m_RockeFireTimer < 0.1f) return;

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
		GridManager::Instance().RunningSequences--;
		GridManager::Instance().OnTileDestroy(m_Tile);
	}

	m_RockeFireTimer = 0.0f;

}

const Texture2D* Harpoon::GetTexture() const
{
	return m_Vertical ? s_HarpoonVTexture.get() : s_HarpoonHTexture.get();
}

void Harpoon::OnHit(int damage)
{
	OnClick();
}
