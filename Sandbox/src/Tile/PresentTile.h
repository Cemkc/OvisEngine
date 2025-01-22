#pragma once

#include "Tile.h"

class PresentTile : public Tile
{
	// Inherited via Tile
	void Init(int col, int row) override;
	void SetTileObject(TileObject& tileObject) override;
	void SetTileObject(TileObjectType tileObjectType) override;
	void DestroyTileObject() override;
};

