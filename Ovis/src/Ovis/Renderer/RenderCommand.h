#pragma once

#include "RendererAPI.h"

namespace Ovis
{
	class RenderCommand
	{
	private:
		static RendererAPI* s_RendererAPI;
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(glm::uvec4 color = glm::vec4(0.8f, 0.2f, 0.8f, 1.0f))
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	};
}

