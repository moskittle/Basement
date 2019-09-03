#include "bmpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <string>

namespace Basement {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		BM_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;
		
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		
		//glTextureStorage2D(m_TextureID, 1, GL_RGBA, m_Width, m_Height);

		// format check
		std::string::size_type posfix = path.find(".");
		std::string format = path.substr(posfix+1);
		if (format == "jpg")
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (format == "png")
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTexture(slot, m_TextureID);
	}

}