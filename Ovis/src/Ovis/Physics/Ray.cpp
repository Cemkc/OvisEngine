#include "ovpch.h"
#include "Ray.h"
#include "Ovis/Utils/MatrixUtils.h"

namespace Ovis
{
	Ray::Ray(glm::vec3 origin, glm::vec3 direction)
		:m_Origin(origin), m_Direction(direction)
	{
	}

	bool Ovis::Ray::Cast(Ray& ray, const std::vector<GameEntity*>& entities, std::vector<GameEntity*>& hitEntities)
	{
		bool rayHit = false;

		for each(GameEntity* entity in entities)
		{
			Transform& transform = entity->GetTransform();
			glm::mat4 transInverse = glm::inverse(transform.GetTransformationMatrix());
			//OV_CORE_INFO("Matrix:\n{0}", Utils::Mat4ToString(transform.GetTransformationMatrix()));
			//OV_CORE_INFO("Inverse Matrix:\n{0}", Utils::Mat4ToString(transInverse));
			glm::vec3 localOrigin = transInverse * glm::vec4(ray.m_Origin, 1.0f);
			//OV_CORE_INFO("Local Origin:\n({0}, {1}, {2})", localOrigin.x, localOrigin.y, localOrigin.z);
			glm::vec3 localDirection = glm::normalize(transInverse * glm::vec4(ray.m_Direction, 0.0f));
			//OV_CORE_INFO("Local Direction:\n({0}, {1}, {2})", localDirection.x, localDirection.y, localDirection.z);

			// Step 3: Ray-plane intersection (Z = 0 in local space)
			if (glm::abs(localDirection.z) < 1e-6) continue; // Ray is parallel to the plane

			float t = -localOrigin.z / localDirection.z;
			if (t < 0) continue; // Intersection behind the ray

			// Step 4: Calculate intersection point
			glm::vec3 localHitPoint = localOrigin + t * localDirection;

			// Step 5: Check if the point is within the quad bounds
			glm::vec2 halfSize = glm::vec2(1.0f) * 0.5f; // Assuming quad size is 1x1 in local space
			if (localHitPoint.x >= -halfSize.x && localHitPoint.x <= halfSize.x &&
				localHitPoint.y >= -halfSize.y && localHitPoint.y <= halfSize.y)
			{
				// Transform hit point back to world space
				hitEntities.push_back(entity);
				rayHit = true;
			}
		}
		
		return rayHit;
	}
}

