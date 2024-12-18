#include "ovpch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Ovis
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int widht, height, nrChannels;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* imageData = stbi_load(path.c_str(), &widht, &height, &nrChannels, 0);
		OV_CORE_ASSERT(imageData, "Failed to load image!");
		m_Widht = widht;
		m_Height = height;

		unsigned int texture;
		glGenTextures(1, &m_RendererId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Widht, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(imageData);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererId);
	}
}