#include "bmpch.h"
#include "OpenGLTexture.h"
#include "OpenGLDebug.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <string>

namespace Basement {

	//----------------------------------------------------
	// OpenGL Texture 2D
	//----------------------------------------------------
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

		auto [internalFormat, dataFormat] = GetFormat(channels);
		BM_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalFormat, m_Width, m_Height);
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		SetTextureParameter();

		stbi_image_free(data);
	}

	void OpenGLTexture2D::SetTextureParameter()
	{
		if (m_IsRepeated)
		{
			OpenGLCall(glGenerateTextureMipmap(m_TextureID));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		}
		else
		{
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			OpenGLCall(glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	std::tuple<u32, u32> OpenGLTexture2D::GetFormat(u32 channels) const
	{
		u32 internalFormat = 0, dataFormat = 0;
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

		return { internalFormat, dataFormat };
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



	//----------------------------------------------------
	// OpenGL Texture Cube
	//----------------------------------------------------


	OpenGLTextureCube::OpenGLTextureCube(const std::string& directory, const std::string& format) :
		m_Directory(directory),
		m_Format(format)
	{
		std::vector<std::string> faces = { "right", "left", "top", "bottom" , "front", "back" };
		std::string path;

		OpenGLCall(glGenTextures(1, &m_TextureID));
		OpenGLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID));
		
		int width, height, channels;
		for (u8 i = 0; i < 6; ++i)
		{
			path = directory + "/" + faces[i] + + "." + format;

			stbi_set_flip_vertically_on_load(false);
			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			BM_CORE_ASSERT(data, "Failed to load image!");

			auto [internalFormat, dataFormat] = GetFormat(channels);
			BM_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");
			
			OpenGLCall(glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data));
			stbi_image_free(data);
		}

		SetTextureParameter();
	}

	std::tuple<u32, u32> OpenGLTextureCube::GetFormat(u32 channels) const
	{
		u32 internalFormat = 0, dataFormat = 0;
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
		return { internalFormat, dataFormat };
	}

	void OpenGLTextureCube::Bind(u32 slot) const
	{
		OpenGLCall(glBindTextureUnit(slot, m_TextureID););
	}

	void OpenGLTextureCube::Activate(u32 unit) const
	{
		OpenGLCall(glActiveTexture(GL_TEXTURE0 + unit));
		OpenGLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID));
	}


	void OpenGLTextureCube::SetTextureParameter()
	{
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		glDeleteTextures(1, &m_TextureID);
	}

}