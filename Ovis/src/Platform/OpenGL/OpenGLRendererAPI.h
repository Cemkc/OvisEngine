#pragma once

#include "Ovis/Renderer/RendererAPI.h"

namespace Ovis
{
	class OpenGLRendererAPI : public RendererAPI
	{
	private:
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		void Init() override;
		void SetViewport(int width, int height) override;
		void SetClearColor(glm::uvec4 color) override;
		void Clear() override;

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}
