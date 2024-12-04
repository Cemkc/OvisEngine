#include "ovpch.h"

#include "OpenGLShader.h"
#include <glad/glad.h>

namespace Ovis 
{
	OpenGLShader::OpenGLShader(const std::string vertexSrc, const std::string fragmentSrc)
	{
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const char* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, nullptr);
		glCompileShader(vertexShader);

		int isCompiled;
		char infoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (!isCompiled)
		{
			int maxLenght = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLenght);

			std::vector<char> infoLog(maxLenght);
			glGetShaderInfoLog(vertexShader, maxLenght, &maxLenght, &infoLog[0]);

			glDeleteShader(vertexShader);

			OV_CORE_ERROR("{0}", infoLog.data());
			OV_CORE_ASSERT(false, "Vertex shader compilation failure!");
		}

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (!isCompiled)
		{
			int maxLenght = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLenght);

			std::vector<char> infoLog(maxLenght);
			glGetShaderInfoLog(fragmentShader, maxLenght, &maxLenght, &infoLog[0]);

			glDeleteShader(fragmentShader);

			OV_CORE_ERROR("{0}", infoLog.data());
			OV_CORE_ASSERT(false, "Fragment shader compilation failure!");
		}

		m_ShaderProgram = glCreateProgram();
		glAttachShader(m_ShaderProgram, vertexShader);
		glAttachShader(m_ShaderProgram, fragmentShader);
		glLinkProgram(m_ShaderProgram);

		int isLinked = 0;
		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, (int*)&isLinked);
		if (!isLinked)
		{
			int maxLength = 0;
			glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(m_ShaderProgram, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_ShaderProgram);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			OV_CORE_ERROR("{0}", infoLog.data());
			OV_CORE_ASSERT(false, "Shader program link failure!");
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_ShaderProgram, vertexShader);
		glDetachShader(m_ShaderProgram, fragmentShader);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ShaderProgram);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}
}