#pragma once

#include "Ovis.h"
#include "Block.h"

class PurpleTile : public Block
{
public:
	PurpleTile()
	{
		m_Name = "Purple Tile";
		m_Type = TileObjectType::Purple;
		m_Color = { 0.627f, 0.125f, 0.941f, 1.0f };
	}
};
