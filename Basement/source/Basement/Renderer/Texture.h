#pragma once

#include <string>

#include "Basement/Core/core.h"

namespace Basement {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual u32 GetTextureID() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;

		virtual void SetTypeName(const std::string& typeName) = 0;
		virtual std::string GetTypeName() const = 0;

		virtual void Bind(u32 slot = 0) const = 0;
		virtual void Activate(u32 unit = 0) const = 0;

		static SharedPtr<Texture2D> Create(const std::string& path, bool isRepeated = false);
	};

	class TextureCube : public Texture
	{
	public:
		virtual ~TextureCube() = default;

		static SharedPtr<TextureCube> Create(const std::string& directory, const std::string& format);

		virtual void Bind(u32 slot = 0) const = 0;
		virtual void Activate(u32 unit = 0) const = 0;
	};

}