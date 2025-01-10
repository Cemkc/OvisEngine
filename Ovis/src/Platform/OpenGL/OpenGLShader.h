#pragma once

#include "Ovis/Renderer/Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Ovis 
{
	class OpenGLShader : public Shader
	{
	private:
		unsigned int m_RendererId;
		std::string m_Name;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		virtual ~OpenGLShader() override;

		std::string GetName() override { return m_Name; }

		void Bind() const override;
		void UnBind() const override;

		// Inherited via Shader
		void SetUniform(const std::string& name, int value) override;
		void SetUniform(const std::string& name, int* value, uint32_t count) override;
		void SetUniform(const std::string& name, float value) override;
		void SetUniform(const std::string& name, float v0, float v1, float v2, float v3) override;
		void SetUniform(const std::string& name, glm::vec4 vec) override;
		void SetUniform(const std::string& name, const glm::mat4& mat) override;
		int GetUniformLocation(const std::string& name);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	};
}