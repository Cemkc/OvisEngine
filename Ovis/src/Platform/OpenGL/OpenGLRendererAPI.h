#pragma once

#include "Ovis/Renderer/RendererAPI.h"

namespace Ovis
{
	class OpenGLRendererAPI : public RendererAPI
	{
		void SetClearColor(glm::uvec4 color) override;
		void Clear() override;

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}



