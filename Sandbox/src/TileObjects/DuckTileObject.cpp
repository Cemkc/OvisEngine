#include "ovpch.h"

#include "Managers/GridManager.h"
#include "DuckTileObject.h"

bool DuckTileObject::s_DuckIdArray[GridManager::TileCount()] = {false};

DuckTileObject::DuckTileObject()
{
	m_Name = "Duck Tile";
	m_Type = TileObjectType::Duck;
	m_Color = glm::vec4(0.573f, 0.149f, 0.941f, 1.0f);

	for (int i = 0; i < GridManager::TileCount(); i++)
	{
		if (s_DuckIdArray[i] == 0)
		{
			s_DuckIdArray[i] = true;
			m_DuckId = i;
			break;
		}
	}

	GridManager::Instance().AddEventCallback(m_DuckId, std::bind(&DuckTileObject::OnGridEvent, this, std::placeholders::_1));
}

DuckTileObject::~DuckTileObject()
{
	GridManager::Instance().RemoveEventCallback(m_DuckId);
	s_DuckIdArray[m_DuckId] = false;
}

void DuckTileObject::OnGridEvent(GridEvent& event)
{
	if (event.GetEventType() == GridEventType::FillEnd)
		OnFillEnd();
}

void DuckTileObject::OnFillEnd()
{
	OV_INFO("Heard Fill End!");
	if (m_Tile->GetTilePos().y == 0)
	{
		GridManager::Instance().OnTileDestroy(m_Tile);
	}
}
