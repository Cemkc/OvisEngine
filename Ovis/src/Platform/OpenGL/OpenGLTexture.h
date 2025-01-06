#pragma once

#include "Ovis/Renderer/Texture.h"

#include <glad/glad.h>

namespace Ovis
{
	class OpenGLTexture2D : public Texture2D
	{
	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererId;
		GLenum m_InternalFormat, m_DataFormat;
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		void Bind(uint32_t slot) const override;
	};
}
