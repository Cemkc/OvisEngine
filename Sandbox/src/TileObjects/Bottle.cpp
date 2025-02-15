#include "ovpch.h"

#include "Bottle.h"
#include "Managers/GridManager.h"

std::shared_ptr<Texture2D> Bottle::s_BottleTexture = nullptr;

Bottle::Bottle()
{
	m_Name = "Bottle";
	m_Type = TileObjectType::Bottle;
	m_Category = (TileObjectCategory)(m_Category | TileObjectCategory::MatchSensitiveObject | TileObjectCategory::HitableTileObject);
	m_Color = glm::vec4(0.573f, 0.149f, 0.941f, 1.0f);

	if (!s_BottleTexture)
	{
		s_BottleTexture = Texture2D::Create("assets/textures/Bottle.png");
	}
}

const Texture2D* Bottle::GetTexture() const
{
	return s_BottleTexture.get();
}

void Bottle::OnHit(int damage)
{
	GridManager::Instance().OnTileDestroy(m_Tile);
}

void Bottle::OnMatchHit()
{
	GridManager::Instance().OnTileDestroy(m_Tile);
}
