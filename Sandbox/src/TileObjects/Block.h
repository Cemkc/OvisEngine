#pragma once

#include "ClickableTileObject.h"

class Block : public ClickableTileObject
{
public:
	virtual int GetCategoryFlags() const override { 
		OV_INFO("In block!");
		return ClickableTileObject::GetCategoryFlags(); 
	}

	virtual bool OnClick() override;
};