#pragma once

#include "Ovis.h"
#include "Block.h"

using namespace Ovis;

class BlueTile : public Block
{
public:
	BlueTile()
	{
		m_Name = "Blue Tile";
		m_Type = TileObjectType::Blue;
		m_Color = { 0.2f, 0.2f, 0.8f, 1.0f };
	}
};