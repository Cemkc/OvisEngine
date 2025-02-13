#include "ovpch.h"

#include "Anchor.h"
#include "Managers/GridManager.h"

std::shared_ptr<Texture2D> Anchor::s_AnchorTexture = nullptr;

Anchor::Anchor()
{
	m_Name = "Anchor";
	m_Type = TileObjectType::Anchor;
	m_Color = glm::vec4(0.573f, 0.149f, 0.941f, 1.0f);

	if (!s_AnchorTexture)
	{
		s_AnchorTexture = Texture2D::Create("assets/textures/Anchor.png");
	}
}

const Texture2D* Anchor::GetTexture() const
{
	return s_AnchorTexture.get();
}

void Anchor::OnFillEnd()
{
	OV_INFO("Heard Fill End!");
	if (m_Tile->GetTilePos().y == 0)
	{
		GridManager::Instance().OnTileDestroy(m_Tile);
	}
}
