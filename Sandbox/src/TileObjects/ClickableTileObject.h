#pragma once

#include "TileObject.h"

class ClickableTileObject : public TileObject
{
public:
	virtual bool OnClick() = 0;

	virtual int GetCategoryFlags() const override { return TileObjectCategory::ClickableCategory; }
};