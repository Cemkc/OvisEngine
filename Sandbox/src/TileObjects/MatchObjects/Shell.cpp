#include "Shell.h"

std::shared_ptr<Texture2D> Shell::s_ShellTexture = nullptr;

Shell::Shell()
{
	m_Name = "Shell";
	m_Type = TileObjectType::Shell;
	m_Color = { 0.2f, 0.2f, 0.8f, 1.0f };

	if (!s_ShellTexture)
	{
		s_ShellTexture = Texture2D::Create("assets/textures/Shell.png");
	}
}

const Texture2D* Shell::GetTexture() const
{
	return s_ShellTexture.get();
}
