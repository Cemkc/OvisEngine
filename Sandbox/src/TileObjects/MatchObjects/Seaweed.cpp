#include "ovpch.h"

#include "Seaweed.h"

std::shared_ptr<Texture2D> Seaweed::s_SeaweedTexture = nullptr;

Seaweed::Seaweed()
{
	m_Name = "Seaweed";
	m_Type = TileObjectType::Seaweed;
	m_Color = { 0.2f, 0.8f, 0.2f, 1.0f };

	if (!s_SeaweedTexture)
		s_SeaweedTexture = Texture2D::Create("assets/textures/Seaweed.png");

	m_Texture = s_SeaweedTexture.get();
}
