#pragma once

#include "Ovis.h"
#include "TileTypes.h"

using namespace Ovis;

class Tile;

class TileObject : public GameEntity
{
protected:
	Tile* m_Tile;

	TileObjectType m_Type;
	TileObjectCategory m_Category = TileObjectCategory::None;

public:
	virtual ~TileObject() = default;

	Tile& GetTile() { return *m_Tile; }
	void SetTile(Tile* tile) { m_Tile = tile; }

	TileObjectType GetTileObjectType() { return m_Type; }
	TileObjectCategory GetTileObjectCategory() { return m_Category; }

	inline bool IsInCategory(TileObjectCategory category) 
	{
		return m_Category & category;
	}
};