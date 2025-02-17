#pragma once

#include "Ovis.h"
#include "Block.h"

using namespace Ovis;

class Star : public Block
{
private:
	static std::shared_ptr<Texture2D> s_StarTexture;
public:
	Star();
};
