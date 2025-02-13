#pragma once

#include "Ovis.h"
#include "Block.h"

class YellowTile : public Block
{
public:
	YellowTile()
	{
		m_Name = "Yellow Tile";
		m_Type = TileObjectType::Yellow;
		m_Color = { 0.85f, 0.85f, 0.1f, 1.0f };
	}
};

