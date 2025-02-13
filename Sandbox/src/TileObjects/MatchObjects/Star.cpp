#include "Star.h"

std::shared_ptr<Texture2D> Star::s_StarTexture = nullptr;

Star::Star()
{
	m_Name = "Purple";
	m_Type = TileObjectType::Star;
	m_Color = { 0.627f, 0.125f, 0.941f, 1.0f };

	if (!s_StarTexture)
	{
		s_StarTexture = Texture2D::Create("assets/textures/Star.png");
	}
}

const Texture2D* Star::GetTexture() const
{
	return s_StarTexture.get();
}
