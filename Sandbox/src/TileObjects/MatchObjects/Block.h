#pragma once

#include "ClickableTileObject.h"
#include "IHitableTileObject.h"

class Block : public ClickableTileObject, public IHitableTileObject
{
public:
	Block();
	bool OnClick() override;
	void OnHit(int damage) override;
};