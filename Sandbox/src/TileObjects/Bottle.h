#pragma once

#include "TileObject.h"
#include "IMatchSensitive.h"

class Bottle : public TileObject, public IMatchSensitive
{
private:
	static std::shared_ptr<Texture2D> s_BottleTexture;
public:
	Bottle();

	const Texture2D* GetTexture() const override;

	void OnMatchHit() override;
};

