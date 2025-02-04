#pragma once

#include "ClickableTileObject.h"

class Block : public ClickableTileObject
{
public:
	virtual bool OnClick() override;
};