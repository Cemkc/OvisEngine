#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "Shader.h"

namespace Ovis
{
	class RendererAPI
	{
	public:
		enum API
		{
			None = 0,
			OpenGL = 1,
		};

	private:
		static API s_API;

	public:
		virtual void SetClearColor(glm::uvec4 color = glm::vec4(0.8f, 0.2f, 0.8f, 1.0f)) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	};
}


