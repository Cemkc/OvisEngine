#pragma once

#include "Ovis.h"
#include "TileObjects/TileObject.h"

using namespace Ovis;

class Tile
{
protected:
	TileObject* m_TileObject;
	glm::ivec2 m_TilePos;
	int m_TileId;

public:
	glm::ivec2 GetTilePos(){ return m_TilePos; }
	int GetTileId() { return m_TileId; }

	virtual void Init(int col, int row) = 0;
	virtual void SetTileObject(TileObject& tileObject) = 0;
	virtual void SetTileObject(TileObjectType tileObjectType) = 0;
	virtual void DestroyTileObject() = 0;
	
	inline TileObject& GetTileObject() { return *m_TileObject; }
};
