#pragma once

#include "Ovis.h"
#include "Block.h"

using namespace Ovis;

class BlueTile : public Block
{
public:
	BlueTile(){
		m_Type = TileObjectType::Blue;
	}

	virtual int GetCategoryFlags() const override { return Block::GetCategoryFlags(); }
};