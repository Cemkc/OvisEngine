#pragma once

#include <glm/glm.hpp>
#include "Ovis/GameEntity/GameEntity.h"

namespace Ovis
{
	class Ray
	{
	public:
		Ray(glm::vec3 origin, glm::vec3 direction);
		static bool Ovis::Ray::Cast(Ray& ray, const std::vector<GameEntity*>& entities, std::vector<GameEntity*>& hitEntities);
	private:
		glm::vec3 m_Origin;
		glm::vec3 m_Direction;
	};

}


