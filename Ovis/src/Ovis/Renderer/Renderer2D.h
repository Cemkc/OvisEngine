#pragma once

#include "Ovis/Renderer/Camera.h"
#include "Ovis/Renderer/Texture.h"
#include "Ovis/GameEntity/Transform.h"

namespace Ovis
{
	class Renderer2D
	{
	private:
		static Renderer2D* s_Instance;
	public:
		Renderer2D();

		inline static Renderer2D& Instance(){ return *s_Instance; }

		virtual ~Renderer2D() = default;

		virtual void Init() = 0;
		virtual void ShutDown() = 0;

		virtual void BeginScene(const OrthographicCamera& camera) = 0;
		virtual void EndScene() = 0;

		virtual void SubmitQuad(const Transform& transform, const glm::vec4& color) = 0;
		virtual void SubmitQuad(const Transform& transform, const Texture2D& texture, float tilingFactor = 1.0f) = 0;
	};
}

