#include "bmpch.h"
#include "OpenGLTexture.h"
#include "OpenGLDebug.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <string>

namespace Basement {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, bool isRepeated) : 
		m_Path(path),
		m_IsRepeated(isRepeated)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		BM_CORE_ASSERT(data, "Failed to load image!");
		
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		BM_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);

		if (m_IsRepeated)
		{
			OpenGLCall(glGenerateTextureMipmap(m_TextureID));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		}
		else 
		{
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		}
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLTexture2D::Bind(u32 slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	void OpenGLTexture2D::Activate(u32 unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

}