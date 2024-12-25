#pragma once

#include "Ovis/Renderer/Camera.h"
#include "Ovis/Renderer/Texture.h"
#include "Ovis/GameEntity/Transform.h"

namespace Ovis
{
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const Transform& transform, const glm::vec4& color);
		static void DrawQuad(const Transform& transform, const Texture2D& texture);
	};
}

