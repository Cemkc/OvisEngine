#pragma once

#include "Ovis/Renderer/Shader.h"

namespace Ovis 
{
	class OpenGLShader : public Shader
	{
	private:
		unsigned int m_ShaderProgram;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		OpenGLShader(const std::string vertexSrc, const std::string fragmentSrc);

		void Bind() const override;
		void UnBind() const override;

		// Inherited via Shader
		void SetUniform(const std::string& name, int value) override;
		void SetUniform(const std::string& name, float value) override;
		void SetUniform(const std::string& name, float v0, float v1, float v2, float v3) override;
		void SetUniform(const std::string& name, glm::vec4 vec) override;
		void SetUniform(const std::string& name, const glm::mat4& mat) override;
		int GetUniformLocation(const std::string& name);
	};
}