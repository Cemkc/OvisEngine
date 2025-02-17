#pragma once

#include "Ovis.h"
#include "Block.h"

using namespace Ovis;

class Shell : public Block
{
private:
	static std::shared_ptr<Texture2D> s_ShellTexture;
public:
	Shell();
};