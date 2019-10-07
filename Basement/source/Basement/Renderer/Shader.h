#pragma once

#include <string>

namespace Basement {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static SharedPtr<Shader> Create(const std::string& path);
		static SharedPtr<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragSource);

		virtual const std::string& GetName() const = 0;
	private:
		u32 m_ProgramID = 0;
	};

	class ShaderLibrary
	{
	public:
		SharedPtr<Shader> Load(const std::string& filePath);
		SharedPtr<Shader> Load(const std::string& name, const std::string& filePath);
		SharedPtr<Shader> Get(const std::string& name);
	private:
		void Add(const SharedPtr<Shader>& shader);
		void Add(const std::string& name, const SharedPtr<Shader>& shader);
		bool ShaderExists(const std::string& name) const;
	private:
		std::unordered_map<std::string, SharedPtr<Shader>> m_ShaderMap;
	};

}