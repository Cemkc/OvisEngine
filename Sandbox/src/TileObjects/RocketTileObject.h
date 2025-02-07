#pragma once

#include "ClickableTileObject.h"
#include "IHitableTileObject.h"

class RocketTileObject : public ClickableTileObject, public IHitableTileObject
{
public:
	RocketTileObject();
	bool OnClick() override;
	void OnUpdate();

private:
	float m_RockeFireTimer = 0.0f;

	bool m_Active = false;
	bool m_Vertical;

	glm::vec2 m_TileAPos;
	glm::vec2 m_TileBPos;

	void OnHit(int damage) override;
};