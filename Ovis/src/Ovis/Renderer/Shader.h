#pragma once

#include <glm/glm.hpp>

namespace Ovis
{
	class Shader 
	{
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Shader* Create(const std::string& filepath);
		static Shader* Create(const std::string& vertexScr, const std::string& fragmentScr);

		virtual ~Shader() = default;

		virtual void SetUniform(const std::string& name, int value) = 0;

		virtual void SetUniform(const std::string& name, float value) = 0;

		virtual void SetUniform(const std::string& name, float v0, float v1, float v2, float v3) = 0;

		virtual void SetUniform(const std::string& name, glm::vec4 vec) = 0;

		virtual void SetUniform(const std::string& name, const glm::mat4& mat) = 0;
	};
}
