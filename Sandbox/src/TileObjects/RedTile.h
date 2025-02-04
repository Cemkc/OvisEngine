#pragma once

#include "Ovis.h"
#include "Block.h"

using namespace Ovis;

class RedTile : public Block
{
public:
	RedTile()
	{
		m_Name = "Red Tile";
		m_Type = TileObjectType::Red;
		m_Color = { 0.8f, 0.2f, 0.2f, 1.0f };
	}
};