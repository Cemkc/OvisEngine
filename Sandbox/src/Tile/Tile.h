#pragma once

#include "Ovis.h"
#include "TileTypes.h"

using namespace Ovis;

class TileObject;

class Tile : public GameEntity
{
protected:
	std::shared_ptr<TileObject> m_TileObject;
	glm::ivec2 m_TilePos;
	int m_TileId;

public:
	Tile() : GameEntity(){}
	Tile(std::string name) : GameEntity(name){}

	glm::ivec2 GetTilePos(){ return m_TilePos; }
	int GetTileId() { return m_TileId; }

	virtual void Init(int col, int row) = 0;
	virtual void SetTileObject(const std::shared_ptr<TileObject> tileObject) = 0;
	virtual void DestroyTileObject() = 0;
	
	std::shared_ptr<TileObject> GetTileObject() {
		return m_TileObject;
	}
};
