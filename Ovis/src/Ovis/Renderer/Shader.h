#pragma once

namespace Ovis
{
	class Shader 
	{
	private:
		uint32_t m_RendererId;
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Shader* Create(const std::string vertexScr, const std::string fragmentScr);
	};
}