#pragma once

#include <glm/glm.hpp>

namespace Ovis
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TextureId;
		float TilingFactor;
	};
}