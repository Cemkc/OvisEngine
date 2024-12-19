#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

namespace Ovis
{
	class Shader 
	{
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static std::shared_ptr<Shader> Create(const std::string& filepath);
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexScr, const std::string& fragmentScr);

		virtual ~Shader() = default;

		virtual std::string GetName() = 0;

		virtual void SetUniform(const std::string& name, int value) = 0;

		virtual void SetUniform(const std::string& name, float value) = 0;

		virtual void SetUniform(const std::string& name, float v0, float v1, float v2, float v3) = 0;

		virtual void SetUniform(const std::string& name, glm::vec4 vec) = 0;

		virtual void SetUniform(const std::string& name, const glm::mat4& mat) = 0;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
		void Add(const std::shared_ptr<Shader>& shader);
		std::shared_ptr<Shader> Load(const std::string& filepath);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Shader> Get(const std::string& name);
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}
