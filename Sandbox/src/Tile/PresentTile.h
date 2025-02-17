#pragma once

#include "Tile.h"

class PresentTile : public Tile
{
private:
	static std::shared_ptr<Texture2D> s_TileTexture;
public:
	PresentTile() : Tile() {}
	PresentTile(std::string name) : Tile(name){}

	void Init(int col, int row) override;

	void SetTileObject(const std::shared_ptr<TileObject> tileObject) override;

	void DestroyTileObject() override;
};

