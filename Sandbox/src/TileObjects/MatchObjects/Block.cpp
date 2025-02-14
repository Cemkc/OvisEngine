#include "ovpch.h"

#include "Block.h"
#include "Managers/GridManager.h"
#include "IMatchSensitive.h"

Block::Block()
	:ClickableTileObject()
{
	m_Category = (TileObjectCategory)(m_Category | TileObjectCategory::HitableTileObject);
}

bool Block::OnClick()
{
	std::list<int> connectedTiles;
	std::list<int> hitTiles;
	int tileNumber = m_Tile->GetTileId();

	GridManager::Instance().GetConnectedTiles(tileNumber, connectedTiles, hitTiles);

	if (connectedTiles.size() > 1)
	{
		for(int tileNum : hitTiles)
		{
			Tile* tile = GridManager::Instance().GetTile(tileNum);
			TileObject* tileObject = tile->GetTileObject().get();
			OV_INFO("Match Hit: {0}", tileNum);
			if (!tileObject->IsInCategory(TileObjectCategory::MatchSensitiveObject)) continue; // We put same tiles into the list to be able to hit them multiple times but if the tile has gone broke that means we should not do a cast
			OV_INFO("Match Hit detected!");
			dynamic_cast<IMatchSensitive*>(tileObject)->OnMatchHit();
		}

		for (int tileNum : connectedTiles)
		{
			Tile* tile = GridManager::Instance().GetTile(tileNum); // Not that great of a way to to this too many back and forth commuincation and dependency
			GridManager::Instance().OnTileDestroy(tile);
		}

		if (connectedTiles.size() >= 5)
		{
			GridManager::Instance().SetTile(tileNumber, TileObjectType::Rocket);
		}

		return true;
	}

	return false;
}

void Block::OnHit(int damage)
{
	GridManager::Instance().OnTileDestroy(m_Tile);
}
