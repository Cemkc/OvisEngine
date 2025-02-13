#pragma once

#include "Ovis.h"
#include "Block.h"

using namespace Ovis;

class Pearl : public Block
{
private:
	static std::shared_ptr<Texture2D> s_PearlTexture;
public:
	Pearl();

	// Inherited via Block
	const Texture2D* GetTexture() const override;
};