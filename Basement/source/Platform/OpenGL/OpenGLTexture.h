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
		OpenGLTexture2D(const std::string& path, bool isRepeated = false);
		virtual ~OpenGLTexture2D() override;

		virtual u32 GetWidth() const override { return m_Width; }
		virtual u32 GetHeight() const override { return m_Height; }
		virtual u32 GetTextureID() const override { return m_TextureID; }

		virtual void SetTypeName(const std::string& typeName) override { m_TypeName = typeName; }
		virtual std::string GetTypeName() const override { return m_TypeName; }

		virtual void Bind(u32 slot = 0) const override;
		virtual void Activate(u32 unit) const override;
	private:
		void SetTextureParameter();
	private:
		u32 m_Width;
		u32 m_Height;
		u32 m_TextureID;
		std::string m_Path;
		std::string m_TypeName;
		bool m_IsRepeated;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(const std::string& path, const std::string& format);
		virtual ~OpenGLTextureCube() override;

		virtual u32 GetTextureID() const override { return m_TextureID; }
		virtual void Bind(u32 slot) const override;
		virtual void Activate(u32 unit) const override;
	private:
		void SetTextureParameter();
	private:
		u32 m_TextureID;
		std::string m_Directory;
		std::string m_Format;
	};

}