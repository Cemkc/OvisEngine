#pragma once

#include "TileObject.h"

class EmptyTileObject : public TileObject
{
public:
	EmptyTileObject()
	{
		m_Name = "Empty Tile Object";
		m_Type = TileObjectType::None;
		m_Category = TileObjectCategory::None;
		m_Color = { 0.992f, 0.239f, 0.71f, 1.0f };
	}
};