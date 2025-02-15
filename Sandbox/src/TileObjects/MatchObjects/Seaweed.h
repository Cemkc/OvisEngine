#pragma once

#include "Ovis.h"
#include "Block.h"

class Seaweed : public Block
{
private:
	static std::shared_ptr<Texture2D> s_SeaweedTexture;

public:
	Seaweed();

	const Texture2D* GetTexture() const override;

};
