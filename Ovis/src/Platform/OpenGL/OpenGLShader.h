#pragma once

#include "Ovis/Renderer/Shader.h"

namespace Ovis 
{
	class OpenGLShader : public Shader
	{
	private:
		unsigned int m_ShaderProgram;
	public:
		OpenGLShader(const std::string vertexSrc, const std::string fragmentSrc);

		void Bind() const override;
		void UnBind() const override;
	};
}