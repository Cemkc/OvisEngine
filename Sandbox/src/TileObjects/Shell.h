#pragma once

#include "Ovis.h"
#include "Block.h"

using namespace Ovis;

class Shell : public Block
{
public:
	Shell()
	{
		m_Name = "Blue Tile";
		m_Type = TileObjectType::Blue;
		//m_Texture2d = Texture2D::Create("assets/textures/Shell.png");
		m_Color = { 0.2f, 0.2f, 0.8f, 1.0f };
	}
};