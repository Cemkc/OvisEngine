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

		inline static void OnWindowResize(int width, int height)
		{
			s_RendererAPI->SetViewport(width, height);
		}

		inline static void SetClearColor(glm::vec4 color = glm::vec4(0.8f, 0.2f, 0.8f, 1.0f))
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			OV_RENDER_PROFILE_FUNC();
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	};
}

