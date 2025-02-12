#pragma once

#include "Ovis.h"
#include "Block.h"

using namespace Ovis;

class Star : public Block
{
public:
	Star()
	{
		m_Name = "Purple Tile";
		m_Type = TileObjectType::Purple;
		//m_Texture2d = Texture2D::Create("assets/textures/Star.png");
		m_Color = { 0.627f, 0.125f, 0.941f, 1.0f };
	}
};
