#include "PresentTile.h"
#include "Managers/GridManager.h"

void PresentTile::Init(int col, int row)
{
	m_TilePos = glm::ivec2(col, row);
	m_TileId = GridManager::s_GridDimension * col + row;
}

void PresentTile::SetTileObject(TileObject& tileObject)
{
	
}

void PresentTile::SetTileObject(TileObjectType tileObjectType)
{
}

void PresentTile::DestroyTileObject()
{
}
