#include "ovpch.h"

#include "Managers/GridManager.h"
#include "DuckTileObject.h"

DuckTileObject::DuckTileObject()
{
	m_Name = "Duck Tile";
	m_Type = TileObjectType::Duck;
	m_Color = glm::vec4(0.573f, 0.149f, 0.941f, 1.0f);
}

DuckTileObject::~DuckTileObject()
{
}

void DuckTileObject::OnFillEnd()
{
	OV_INFO("Heard Fill End!");
	if (m_Tile->GetTilePos().y == 0)
	{
		GridManager::Instance().OnTileDestroy(m_Tile);
	}
}
