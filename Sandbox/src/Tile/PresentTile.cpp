#include "PresentTile.h"
#include "Managers/GridManager.h"

void PresentTile::Init(int col, int row)
{
	m_Color = { 0.2f, 0.2f, 0.2f, 1.0f };
	m_TilePos = glm::ivec2(col, row);
	m_TileId = GridManager::GridDimension() * col + row;
}

void PresentTile::SetTileObject(const std::shared_ptr<TileObject> tileObject)
{
	m_TileObject = tileObject;
	tileObject->SetTile(this);
}

void PresentTile::DestroyTileObject()
{
	m_TileObject = nullptr;
}
