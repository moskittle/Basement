#pragma once

#include <string>

namespace Basement {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shared<Shader> Create(const std::string& path);
		static Shared<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragSource);

		virtual const std::string& GetName() const = 0;
	private:
		uint32_t m_ProgramID = 0;
	};

	class ShaderLibrary
	{
	public:
		Shared<Shader> Load(const std::string& filePath);
		Shared<Shader> Load(const std::string& name, const std::string& filePath);
		Shared<Shader> Get(const std::string& name);
	private:
		void Add(const Shared<Shader>& shader);
		void Add(const std::string& name, const Shared<Shader>& shader);
		bool ShaderExists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Shared<Shader>> m_ShaderMap;
	};



}