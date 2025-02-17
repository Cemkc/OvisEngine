#pragma once

#include "GameEntity.h"

#include <random>
#include <unordered_map>
#include <list>

namespace Ovis
{
	class IDGenerator
	{
	private:
		static uint32_t s_NextId;
		static std::unordered_set<uint64_t> s_UsedIds;

	public:
		static uint64_t generateID()
		{
			while (s_UsedIds.count(s_NextId))
			{ // Ensure ID is unique
				s_NextId++;
			}
			s_UsedIds.insert(s_NextId);
			return s_NextId++;
		}

		static void releaseID(uint64_t id)
		{
			s_UsedIds.erase(id); // Allow reusing ID
		}
	};

	class EntityManager
	{
	private:
		static std::unordered_map<uint32_t, GameEntity*> s_GameEntities;

	public:
		static void OnEntityCreate(GameEntity* entity, uint32_t id);
		static void OnEntityDestroy(uint32_t id);

		static const std::unordered_map<uint32_t, GameEntity*>& GetEntityMap() { return s_GameEntities; }
	};
}