#pragma once

#include "TileObject.h"

class ClickableTileObject : public TileObject
{
public:
	ClickableTileObject()
	{
		m_Category = (TileObjectCategory)(m_Category | TileObjectCategory::ClickableCategory);
	}
	virtual bool OnClick() = 0;
};