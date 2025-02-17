#include "ovpch.h"

#include "EntityManager.h"

namespace Ovis
{
	uint32_t IDGenerator::s_NextId = 1;
	std::unordered_set<uint64_t> IDGenerator::s_UsedIds;

	std::unordered_map<uint32_t, GameEntity*> EntityManager::s_GameEntities;

	void EntityManager::OnEntityCreate(GameEntity* entity, uint32_t id)
	{
		s_GameEntities[id] = entity;
	}

	void EntityManager::OnEntityDestroy(uint32_t id)
	{
		s_GameEntities.erase(id);
		IDGenerator::releaseID(id);
	}
}