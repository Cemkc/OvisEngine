#include "ovpch.h"

#include "Tentacle.h"

std::shared_ptr<Texture2D> Tentacle::s_TentacleTexture = nullptr;

Tentacle::Tentacle()
{
	m_Name = "Tentacle";
	m_Type = TileObjectType::Tentacle;
	m_Color = { 0.85f, 0.85f, 0.1f, 1.0f };

	if (!s_TentacleTexture)
		s_TentacleTexture = Texture2D::Create("assets/textures/Tentacle.png");
}

const Texture2D* Tentacle::GetTexture() const
{
	return s_TentacleTexture.get();
}
