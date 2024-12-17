#pragma once

#include  <glm/glm.hpp>

namespace Ovis
{
	struct Transform
	{
	public:
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
	};
}
