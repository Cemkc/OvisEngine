#pragma once

#include "TileObject.h"

class Anchor : public TileObject
{
private:
	static std::shared_ptr<Texture2D> s_AnchorTexture;
public:
	Anchor();

	void OnFillEnd();
};
