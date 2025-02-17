#include "PresentTile.h"
#include "Managers/GridManager.h"

std::shared_ptr<Texture2D> PresentTile::s_TileTexture = nullptr;

void PresentTile::Init(int col, int row)
{
	m_Color = { 0.2f, 0.2f, 0.2f, 0.6f };
	m_TilePos = glm::ivec2(col, row);
	m_TileId = GridManager::GridDimension() * col + row;

	if (!s_TileTexture)
		s_TileTexture = Texture2D::Create("assets/textures/borders.png");

	m_Texture = s_TileTexture.get();
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
