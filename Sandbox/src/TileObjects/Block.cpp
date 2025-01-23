#include "ovpch.h"

#include "Block.h"
#include "Managers/GridManager.h"

bool Block::OnClick()
{
	std::list<int> connectedTiles;
	std::list<int> hitTiles;
	int tileNumber = m_Tile->GetTileId();

	GridManager::Instance().GetConnectedTiles(tileNumber, connectedTiles, hitTiles);

	if (connectedTiles.size() > 1)
	{
		for(int tileNum : connectedTiles)
		{
			Tile& tile = GridManager::Instance().GetTile(tileNum); // Not that great of a way to to this too many back and forth commuincation and dependency
			TileObject& tileObject = tile.GetTileObject();
			OV_INFO("{0}", tile.GetTileId());
			// GridManager.s_Instance.OnTileDestroy(tile, tile.ActiveTileObject());

			//tileObject.OnDestroy?.Invoke(tile, tileObject);
		}

		for(int tileNum : hitTiles)
		{
			Tile& tile = GridManager::Instance().GetTile(tileNum);
			if (!tile.GetTileObject().IsInCategory(TileObjectCategory::MatchSensitiveObject)) continue; // We put same tiles into the list to be able to hit them multiple times but if the tile is gone-broke that means we should not do a cast
			//IMatchSensitive matchSensitiveTile = tile.ActiveTileObject() as IMatchSensitive;
			//matchSensitiveTile.OnMatchHit();
		}

		/*if (connectedTiles.Count >= 5)
		{
			GridManager.s_Instance.GetTile(tileNumber).SetTileObject(TileObjectType.Rocket);
		}*/

		return true;
	}

	return false;
}
