#pragma once

#include "Ovis.h"
#include "TileTypes.h"

using namespace Ovis;

class Tile;

class TileObject : public GameEntity
{
protected:
	std::shared_ptr<Tile> m_Tile;

	TileObjectType m_Type;
	TileObjectCategory m_Category;

public:
	Tile& GetTile() { return *m_Tile; }
	void SetTile(std::shared_ptr<Tile> tile) { m_Tile = tile; }
	TileObjectType GetTileObjectType() { return m_Type; }
	virtual int GetCategoryFlags() const = 0;

	inline bool IsInCategory(TileObjectCategory category) 
	{
		return GetCategoryFlags() & category;
	}
};