#pragma once

#include "Ovis.h"
#include "Block.h"

class Tentacle : public Block
{
private:
	static std::shared_ptr<Texture2D> s_TentacleTexture;
public:
	Tentacle();
};

