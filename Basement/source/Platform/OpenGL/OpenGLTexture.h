#pragma once

#include "Basement/Renderer/Texture.h"

namespace Basement {

	enum class TextureFormat
	{
		NONE
	};

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual const uint32_t& GetTextureID() const override { return m_TextureID; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Activate(uint32_t unit) const override;
	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_TextureID;
	};

}