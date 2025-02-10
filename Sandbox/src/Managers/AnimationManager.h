#pragma once

#include "Ovis.h"
#include "GridManager.h"

using namespace Ovis;

class AnimationManager : public Layer
{
public:
	AnimationManager();

	static AnimationManager* s_Instance;
	inline static AnimationManager& Instance() { return *s_Instance; }

	static bool s_MovingObject;

	void MoveObject(std::shared_ptr<TileObject>& tileObject, Tile* tile);

	void OnUpdate() override;

private:
	static float s_AnimationSpeed;

	struct AnimationData
	{
	public:
		bool Moving;
		std::shared_ptr<TileObject> ObjectToMove;
		Tile* TileToGet;
	};

	std::array<AnimationData, GridManager::TileCount()> m_AnimationArray;
};