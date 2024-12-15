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

	void OpenGLShader::SetUniform(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform(const std::string& name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	}

	void OpenGLShader::SetUniform(const std::string& name, glm::vec4 vec)
	{
		glUniform4f(GetUniformLocation(name), vec.r, vec.g, vec.b, vec.a);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_ShaderProgram, name.c_str());
		if (location == -1)
		{
			std::cout << "WARNING: Uniform " << name << " does not exist" << std::endl;
		}

		m_UniformLocationCache[name] = location;

		return location;
	}
}