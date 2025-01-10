#include "ovpch.h"
#include "OpenGLShader.h"

#include <fstream>

namespace Ovis 
{
	static GLenum GLShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		OV_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	static std::string StringFromGLShaderType(GLenum type)
	{
		if (type == GL_VERTEX_SHADER) return "Vertex";
		if (type == GL_FRAGMENT_SHADER) return "Fragment";

		OV_CORE_ASSERT(false, "Unknown shader type!");
		return "";
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		OV_RENDER_PROFILE_FUNC();

		std::string source = ReadFile(filepath);
		auto sources = PreProcess(source);
		Compile(sources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		OV_RENDER_PROFILE_FUNC();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		OV_RENDER_PROFILE_FUNC();

		glDeleteProgram(m_RendererId);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		OV_RENDER_PROFILE_FUNC();

		std::string content;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end); // Get the file reading pointer to the end of the file
			content.resize(in.tellg()); // Use the pointer that is at the end to figure out the file size and set the string size accordingly
			in.seekg(0, std::ios::beg); // Set the pointer back to the beggining of the file
			in.read(&content[0], content.size()); // Read the file into the string
			in.close(); // Close the stream
		}
		else
		{
			OV_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return content;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		OV_RENDER_PROFILE_FUNC();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLenght = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			OV_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = pos + typeTokenLenght + 1;
			std::string type = source.substr(begin, eol - begin);
			OV_CORE_ASSERT(GLShaderTypeFromString(type), "Invalid shader type specification.");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[GLShaderTypeFromString(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		OV_RENDER_PROFILE_FUNC();

		GLuint program = glCreateProgram();
		OV_CORE_ASSERT(shaderSources.size() <= 5, "Ovis only supports 5 shaders at once");
		std::array<GLuint, 5> shaderIds = { 0 };
		OV_CORE_TRACE(shaderIds.size());
		int glShaderIdIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const char* sourceCstr = source.c_str();
			glShaderSource(shader, 1, &sourceCstr, nullptr);
			glCompileShader(shader);

			int isCompiled;
			char infoLog[512];

			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (!isCompiled)
			{
				int maxLenght = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLenght);

				std::vector<char> infoLog(maxLenght);
				glGetShaderInfoLog(shader, maxLenght, &maxLenght, &infoLog[0]);

				glDeleteShader(shader);

				OV_CORE_ERROR("{0}", infoLog.data());
				OV_CORE_ASSERT(false, "{0} shader compilation failure!", StringFromGLShaderType(type));
				break;
			}
			glAttachShader(program, shader);
			shaderIds[glShaderIdIndex++] = shader;
		}
		
		glLinkProgram(program);

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (!isLinked)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for(auto id : shaderIds)
				glDeleteShader(id);

			OV_CORE_ERROR("{0}", infoLog.data());
			OV_CORE_ASSERT(false, "Shader program link failure!");
		}

		// Always detach shaders after a successful link.
		for (auto id : shaderIds)
			glDetachShader(program, id);

		m_RendererId = program;
	}

	void OpenGLShader::Bind() const
	{
		OV_RENDER_PROFILE_FUNC();

		glUseProgram(m_RendererId);
	}

	void OpenGLShader::UnBind() const
	{
		OV_RENDER_PROFILE_FUNC();

		glUseProgram(0);
	}

	void OpenGLShader::SetUniform(const std::string& name, int value)
	{
		OV_RENDER_PROFILE_FUNC();

		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform(const std::string& name, int* value, uint32_t count)
	{
		OV_RENDER_PROFILE_FUNC();

		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, float value)
	{
		OV_RENDER_PROFILE_FUNC();

		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetUniform(const std::string& name, float v0, float v1, float v2, float v3)
	{
		OV_RENDER_PROFILE_FUNC();

		glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	}

	void OpenGLShader::SetUniform(const std::string& name, glm::vec4 vec)
	{
		OV_RENDER_PROFILE_FUNC();

		glUniform4f(GetUniformLocation(name), vec.r, vec.g, vec.b, vec.a);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat4& mat)
	{
		OV_RENDER_PROFILE_FUNC();

		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		OV_RENDER_PROFILE_FUNC();

		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1)
		{
			std::cout << "WARNING: Uniform " << name << " does not exist" << std::endl;
		}

		m_UniformLocationCache[name] = location;

		return location;
	}
}