#pragma once

#include "Ovis/Renderer/Texture.h"

namespace Ovis
{
	class OpenGLTexture2D : public Texture2D
	{
	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererId;
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		void Bind(uint32_t slot) const override;
	};
}
