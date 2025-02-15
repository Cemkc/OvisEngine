#pragma once

#include "ClickableTileObject.h"
#include "IHitableTileObject.h"

class Harpoon : public ClickableTileObject, public IHitableTileObject
{
public:
	Harpoon();
	bool OnClick() override;
	void OnUpdate();

	const Texture2D* GetTexture() const override;

private:
	static std::shared_ptr<Texture2D> s_HarpoonVTexture;
	static std::shared_ptr<Texture2D> s_HarpoonHTexture;

	float m_RockeFireTimer = 0.0f;

	bool m_Active = false;
	bool m_Fired = false;
	bool m_Vertical;

	glm::vec2 m_TileAPos;
	glm::vec2 m_TileBPos;

	void OnHit(int damage) override;
};