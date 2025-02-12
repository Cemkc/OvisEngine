#pragma once

#include "Ovis.h"
#include "Block.h"

using namespace Ovis;

class Pearl : public Block
{
public:
	Pearl()
	{
		m_Name = "Red Tile";
		m_Type = TileObjectType::Red;
		//m_Texture2d = Texture2D::Create("assets/textures/Pearl.png");
		m_Color = { 0.8f, 0.2f, 0.2f, 1.0f };
	}
};