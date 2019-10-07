#include "bmpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Basement {

	// --Shader---------------------------------------------------

	SharedPtr<Shader> Shader::Create(const std::string& path)
	{
		switch (RendererAPI::GetRendererAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return std::make_shared<OpenGLShader>(path);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI!");
				return nullptr;
		}
	}

	SharedPtr<Shader> Shader::Create(const std::string& name,  const std::string& vertexSource, const std::string& fragSource)
	{
		switch (RendererAPI::GetRendererAPI())
		{
			case RendererAPI::EAPI::None:
				BM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
					return nullptr;
			case RendererAPI::EAPI::OpenGL:
				return std::make_shared<OpenGLShader>(name, vertexSource, fragSource);
			default:
				BM_CORE_ASSERT(false, "Unknown RendererAPI!");
				return nullptr;
		}
	}

	// --Shader Library-----------------------------------------

	void ShaderLibrary::Add(const SharedPtr<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const std::string& name, const SharedPtr<Shader>& shader)
	{
		if (ShaderExists(name))
		{
			BM_CORE_WARN("{1} already exsists in ShaderMap.", name);
		}
		else
		{
			m_ShaderMap[name] = shader;
		}
	}

	bool ShaderLibrary::ShaderExists(const std::string& name) const
	{
		return m_ShaderMap.find(name) != m_ShaderMap.end();
	}

	SharedPtr<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		SharedPtr<Shader> shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	SharedPtr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		SharedPtr<Shader> shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	SharedPtr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		BM_CORE_ASSERT(ShaderExists(name), "Shader({1}) not found!", name);
		return m_ShaderMap[name];
	}

}