#pragma once

#include "TileObject.h"
#include "IMatchSensitive.h"
#include "IHitableTileObject.h"

class Bottle : public TileObject, public IHitableTileObject, public IMatchSensitive
{
private:
	static std::shared_ptr<Texture2D> s_BottleTexture;
public:
	Bottle();

	void OnHit(int damage) override;

	void OnMatchHit() override;
};

