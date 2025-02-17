#include "Pearl.h"

std::shared_ptr<Texture2D> Pearl::s_PearlTexture = nullptr;

Pearl::Pearl()
{
	m_Name = "Pearl";
	m_Type = TileObjectType::Pearl;
	m_Color = { 0.8f, 0.2f, 0.2f, 1.0f };

	if (!s_PearlTexture)
	{
		s_PearlTexture = Texture2D::Create("assets/textures/Pearl.png");
	}

	m_Texture = s_PearlTexture.get();
}
