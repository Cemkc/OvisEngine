#pragma once

#include "Ovis.h"
#include "Block.h"

class GreenTile : public Block
{
public:
	GreenTile()
	{
		m_Name = "Green Tile";
		m_Type = TileObjectType::Green;
		m_Color = { 0.2f, 0.8f, 0.2f, 1.0f };
	}
};
